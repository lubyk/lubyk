/*
  ==============================================================================

   This file is part of the OSCIT library (http://rubyk.org/liboscit)
   Copyright (c) 2007-2009 by Gaspard Bucher - Buma (http://teti.ch).

  ------------------------------------------------------------------------------

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

  ==============================================================================
*/

#include "oscit/osc_command.h"

#include <stdexcept>

#include "osc/OscReceivedElements.h"
#include "osc/OscPacketListener.h"
#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"

#include "oscit/root.h"
#include "oscit/zeroconf_registration.h"
#include "oscit/osc_remote_object.h"


namespace oscit {

#define OSC_OUT_BUFFER_SIZE 2048
#define OSCIT_SERVICE_TYPE "_oscit._udp"

//#define DEBUG_OSC_COMMAND

static void to_stream(osc::OutboundPacketStream &out_stream, const Value &val, bool in_array = false) {
  size_t sz;
  switch (val.type()) {
    case REAL_VALUE:
      out_stream << (float)val.r; // most osc applications don't understand double type tag.
      break;
    case ERROR_VALUE:
      out_stream << val.error_code() << val.error_message().c_str();
      break;
    case STRING_VALUE:
      out_stream << val.c_str();
      break;
    case EMPTY_VALUE: /* continue */
    case NIL_VALUE:
      out_stream << osc::Nil;
      break;
    case LIST_VALUE:
      sz = val.size();
      // do not insert array markers for level 0 type tags: "ff[fs]" not "[ff[fs]]"
      if (in_array) out_stream << osc::ArrayStart;
      for (size_t i = 0; i < sz; ++i) {
        to_stream(out_stream, val[i], true);
      }
      if (in_array) out_stream << osc::ArrayEnd;
      break;
    case ANY_VALUE:
      out_stream << osc::Any;
      break;
    case HASH_VALUE:   /* continue */
      out_stream << (osc::JsonHash)val.to_json().c_str();
    case MATRIX_VALUE: /* continue */
    case MIDI_VALUE:   /* continue */
    default:
      ;// ????
  }
}

static osc::OutboundPacketStream &operator<<(osc::OutboundPacketStream &out_stream, const Value &val) {
  to_stream(out_stream, val);
  return out_stream;
}


class OscCommand::Implementation : public osc::OscPacketListener {
public:

  Implementation(OscCommand *command) : command_(command), socket_(NULL), running_(false) {}

  virtual ~Implementation() {
    kill();
    if (socket_ != NULL) delete socket_;
  }

  void kill() {
    if (running_) socket_->AsynchronousBreak();
    running_ = false;
  }

  /** Send an osc message.
   *  @param remote_endpoint target host.
   */
  void send_message(const Location &remote_endpoint, const char *path, const Value &val) {
    assert(socket_);
    osc::OutboundPacketStream message( osc_buffer_, OSC_OUT_BUFFER_SIZE );
    build_message(path, val, &message);
    try {
      // FIXME: hack oscpack to accept 'Location' or rewrite this layer using ragel...
      socket_->SendTo(IpEndpointName(remote_endpoint.ip(), remote_endpoint.port()), message.Data(), message.Size());
#ifdef DEBUG_OSC_COMMAND
      std::cout << "[" << command_->port() << "] --- " << path << "(" << val << ") --> [" << remote_endpoint << "]" << std::endl;
#endif

    } catch (std::runtime_error &e) {
      std::cerr << "Could not connect to " << remote_endpoint << "\n";
      // TODO: make sure we do not leak here
    }
  }

  /** Start listening for incoming messages (runs in its own thread). */
  void listen() {
    if (socket_ == NULL) {
      try {
        if (command_->port() != 0) {
          socket_ = new UdpListeningReceiveSocket( IpEndpointName( IpEndpointName::ANY_ADDRESS, command_->port() ), this );
        } else {
          std::cout << "Connecting to port 0\n";
          socket_ = new UdpListeningReceiveSocket( IpEndpointName(), this );
          command_->set_port(socket_->BoundPort());
        }
      } catch (std::runtime_error &e) {
        printf("Could not create listening socket on port %i\n", command_->port());
        throw;
      }
    }
#ifdef DEBUG_OSC_COMMAND
    printf("OscCommand listening on port %i\n", command_->port());
#endif
    running_ = true;
    // let's trigger zeroconf registration
    command_->publish_service();

    // done with initializations
    command_->thread_ready();
    socket_->Run();
  }

  void change_port(uint16_t port) {
    try {
      socket_->Bind(IpEndpointName( IpEndpointName::ANY_ADDRESS, port ));
    } catch (std::runtime_error &e) {
      printf("Could not create UdpListeningReceiveSocket on port %i\n", port);
    }
  }

  /** Callback to process incoming messages. */
  virtual void ProcessMessage(const osc::ReceivedMessage &message, const IpEndpointName &ip_end_point) {
    Value res;
    // TODO: reuse location ?
    Url   url(ip_end_point.address, ip_end_point.port, message.AddressPattern());

    Value val(value_from_osc(message));

#ifdef DEBUG_OSC_COMMAND
    std::cout << "[" << command_->port() << "] <-- " << url << "(" << val << ")" << std::endl;
#endif

    ScopedLock lock(command_);
    command_->receive(url, val);
  }

  /** Build an osc message and send it to all observers. */
  void send_to_all(const std::list<Location> locations, const char *path, const Value &val) {
    std::list<Location>::const_iterator it  = locations.begin();
    std::list<Location>::const_iterator end = locations.end();

    osc::OutboundPacketStream message( osc_buffer_, OSC_OUT_BUFFER_SIZE );
    build_message(path, val, &message);

    while (it != end) {

#ifdef DEBUG_OSC_COMMAND
      std::cout << "  " << *it << std::endl;
#endif
      try {
        // FIXME: hack oscpack to use Location or rewrite...
        socket_->SendTo(IpEndpointName(it->ip(), it->port()), message.Data(), message.Size());
        ++it;
      } catch (std::runtime_error &e) {
        std::cerr << "Could not send to observer " << *it << "\n";
      }
    }
  }

  static const char *parse_osc_message(Value &res, const char *start_type_tags, osc::ReceivedMessage::const_iterator &arg) {
    ListValue tmp;
    const char *type_tags = start_type_tags;
    while (*type_tags) {
      switch (*type_tags) {
        case osc::TRUE_TYPE_TAG:
          res.push_back(1.0);
          break;
        case osc::FALSE_TYPE_TAG:
          res.push_back(0.0);
          break;
        case osc::NIL_TYPE_TAG:
          res.push_back(gNilValue);
          break;
        // case osc::INFINITUM_TYPE_TAG:
        //   ??
        case osc::ANY_TYPE_TAG:
          res.push_back(Value('*'));
          break;
        case osc::ARRAY_START_TYPE_TAG:
          tmp.set_type(LIST_VALUE); // clear
          ++type_tags;
          ++arg;
          type_tags = parse_osc_message(tmp, type_tags, arg);
          res.push_back(tmp);
          break;
        case osc::ARRAY_END_TYPE_TAG:
          // return before type_tags increment
          return type_tags;

        // zero length

        case osc::INT32_TYPE_TAG:
          res.push_back(Value((Real)(arg->AsInt32Unchecked())));
          break;
        case osc::FLOAT_TYPE_TAG:
          res.push_back(Value((Real)(arg->AsFloatUnchecked())));
          break;
        case osc::CHAR_TYPE_TAG:
          res.push_back(Value((Real)(arg->AsCharUnchecked())));
          break;
        case osc::DOUBLE_TYPE_TAG:
          res.push_back(Value((Real)(arg->AsDoubleUnchecked())));
          break;
        case osc::HASH_TYPE_TAG:
          res.push_back(Value((Json)arg->AsStringUnchecked()));
          break;
        case osc::STRING_TYPE_TAG:
          res.push_back(Value(arg->AsStringUnchecked()));
          break;
        case osc::RGBA_COLOR_TYPE_TAG:
        case osc::MIDI_MESSAGE_TYPE_TAG:
        case osc::INT64_TYPE_TAG:
        case osc::TIME_TAG_TYPE_TAG:
        case osc::SYMBOL_TYPE_TAG:
        case osc::BLOB_TYPE_TAG:
        default:
          // TODO
          break;
      }
      ++type_tags;
      ++arg;
    }
    return type_tags;
  }

  /** Build a value from osc packet.
   *  @param message osc message.
   *  @return new value corresponding to the osc data.
   */
  static Value value_from_osc(const osc::ReceivedMessage &message) {
    Value res;
    osc::ReceivedMessage::const_iterator arg = message.ArgumentsBegin();
    parse_osc_message(res, message.TypeTags(), arg);
    return res;
  }

  /** Build a message from a value. */
  static void build_message(const char *path, const Value &val, osc::OutboundPacketStream *message) {
    // *message << osc::BeginBundleImmediate << osc::BeginMessage(path) << val << osc::EndMessage << osc::EndBundle;
    *message << osc::BeginMessage(path) << val << osc::EndMessage;
  }
  /** Access to OscCommand.
   */
  OscCommand *command_;

  /** Socket listening to udp packets.
   */
  UdpListeningReceiveSocket *socket_;

  char osc_buffer_[OSC_OUT_BUFFER_SIZE];     /** Buffer used to build osc packets. */
  bool running_;
};


OscCommand::OscCommand(uint16_t port) :
                    Command("osc", "", port) { // no service_type, we do not want automatic registration
  impl_ = new OscCommand::Implementation(this);
}

OscCommand::OscCommand(const char *protocol, const char *service_type, uint16_t port) :
                    Command(protocol, service_type, port) {
  impl_ = new OscCommand::Implementation(this);
}

OscCommand::~OscCommand() {
  kill();
  delete impl_;
}

void OscCommand::kill() {
  impl_->kill();
  join(); // do not kill, impl_->kill() will stop thread.
}

void OscCommand::notify_observers(const char *path, const Value &val) {
#ifdef DEBUG_OSC_COMMAND
      std::cout << "[" << port() << "] - notify -> " << path << "(" << val << ")\n";
#endif
  impl_->send_to_all(observers(), path, val);
}

void OscCommand::listen() {
  impl_->listen();
}

void OscCommand::send_message(const Location &remote_endpoint, const char *path, const Value &val) {
  impl_->send_message(remote_endpoint, path, val);
}

Object *OscCommand::build_remote_object(const Url &url, Value *error) {
  // find host with zeroconf... ? DNS ?
  //   url.host() : url.port()
  //   host not found
  error->set(NOT_FOUND_ERROR, url.str());
  return NULL;
  //   host found ==> IpEndpointName
  // build remoteobject and let it test remote url
  // return remote_objects_->adopt(new OscRemoteObject(this, end_point, url.path()));
}

void OscCommand::change_port(uint16_t port) {
  impl_->change_port(port);
  port_ = port;
}

} // oscit

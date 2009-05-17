#include <stdexcept>

#include "oscit/osc_command.h"
#include "oscit/root.h"
#include "oscit/zeroconf.h"
#include "oscit/osc_remote_object.h"

#include "osc/OscReceivedElements.h"
#include "osc/OscPacketListener.h"
#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"

namespace oscit {

#define OSC_OUT_BUFFER_SIZE 2048
#define OSCIT_SERVICE_TYPE "_oscit._udp"

// #define DEBUG_OSC_COMMAND

static osc::OutboundPacketStream &operator<<(osc::OutboundPacketStream &out_stream, const Value &val) {
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
      for (size_t i = 0; i < sz; ++i) {
        out_stream << val[i];
      }
      break;
    case ANY_VALUE:
      out_stream << osc::Any;
      break;
    case HASH_VALUE:   /* continue */
    case MATRIX_VALUE: /* continue */
    case MIDI_VALUE:   /* continue */
    default:
      ;// ????
  }
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

  /** Add a new satellite to the list of observers. This method is the implementation
   *  of "/.register".
   * TODO: how to get the IP without checking for "/.register" in the 'receive' method ?
   * FIXME: implement TTL (time to live)
   * FIXME: avoid duplicates (??)
   */
  void register_observer(const IpEndpointName &observer) {
    // TODO: check that the observer is not already in the list of observers_.
    observers_.push_back(observer);
  }

  /** Send an osc message.
   *  @param remote_endpoint target host.
   */
  void send(const IpEndpointName &remote_endpoint, const char *url, const Value &val) {
    assert(socket_);
    osc::OutboundPacketStream message( osc_buffer_, OSC_OUT_BUFFER_SIZE );
    build_message(url, val, &message);
    try {
      socket_->SendTo(remote_endpoint, message.Data(), message.Size());
#ifdef DEBUG_OSC_COMMAND
      char address[ IpEndpointName::ADDRESS_AND_PORT_STRING_LENGTH ];
      // get host ip as string
      remote_endpoint.AddressAndPortAsString(address);
      std::cout << "[" << command_->port() << "] --- " << url << "(" << val << ") --> [" << address << "]" << std::endl;
#endif

    } catch (std::runtime_error &e) {
      char address[ IpEndpointName::ADDRESS_AND_PORT_STRING_LENGTH ];
      remote_endpoint.AddressAndPortAsString(address);
      printf("Could not connect to %s\n", address);
      // TODO: make sure we do not leak here
    }
  }

  /** Start listening for incoming messages (runs in its own thread). */
  void listen() {
    if (socket_ == NULL) {
      try {
        socket_ = new UdpListeningReceiveSocket( IpEndpointName( IpEndpointName::ANY_ADDRESS, command_->port() ), this );
      } catch (std::runtime_error &e) {
        printf("Could not create UdpListeningReceiveSocket on port %i\n", command_->port());
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
  virtual void ProcessMessage(const osc::ReceivedMessage &message, const IpEndpointName &remote_endpoint) {
    Value res;
    std::string url(message.AddressPattern());

    if (url == "/.register") {
      register_observer(remote_endpoint);
      // TODO: implement TTL
      // return value ??
    } else {
      Value val(value_from_osc(message));

#ifdef DEBUG_OSC_COMMAND
      char address[ IpEndpointName::ADDRESS_AND_PORT_STRING_LENGTH ];
      // get host ip as string
      remote_endpoint.AddressAndPortAsString(address);
      std::cout << "[" << command_->port() << "] <-- " << url << "(" << val << ") --- [" << address << "]" << std::endl;
#endif
      command_->lock();
        command_->process_message(remote_endpoint, url, val);
      command_->unlock();
    }
  }

  /** Send reply to caller and notify observers. */
  void send_reply(const IpEndpointName &remote_endpoint, const std::string &url, const Value &val) {
    if (val.is_nil()) return;

    if (val.is_error()) {
      send(remote_endpoint, "/.error", val);
    } else {
      // reply to all

      // prepare reply
      Value res(url);
      res.push_back(val);

      send(remote_endpoint, "/.reply", res);

      send_to_observers("/.reply", res, &remote_endpoint); // skip remote_endpoint
    }
  }

  /** Build osc message and send it to all observers. */
  void send_to_observers(const char *url, const Value &val, const IpEndpointName *skip_end_point = NULL) {
    std::list<IpEndpointName>::iterator it  = observers_.begin();
    std::list<IpEndpointName>::iterator end = observers_.end();

    osc::OutboundPacketStream message( osc_buffer_, OSC_OUT_BUFFER_SIZE );
    build_message(url, val, &message);

    while (it != end) {
      if (skip_end_point != NULL && *it == *skip_end_point) {
        // skip
      } else {
        try {
          socket_->SendTo(*it, message.Data(), message.Size());
          ++it;
        } catch (std::runtime_error &e) {
          char address[ IpEndpointName::ADDRESS_AND_PORT_STRING_LENGTH ];
          it->AddressAndPortAsString(address);
          printf("Could not send to observer %s\n", address);
          it = observers_.erase(it);
        }
      }
    }
  }

  /** Build a value from osc packet.
   *  @param message osc message.
   *  @return new value corresponding to the osc data.
   */
  static Value value_from_osc(const osc::ReceivedMessage &message) {
    Value res;
    const char * type_tags = message.TypeTags();
    uint i = 0;
    osc::ReceivedMessage::const_iterator arg = message.ArgumentsBegin();
    osc::ReceivedMessage::const_iterator end = message.ArgumentsEnd();

    while (arg != end) {
      switch (type_tags[i]) {
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
      i++;
      arg++;
    }
    return res;
  }

  /** Build a message from a value. */
  static void build_message(const char *url, const Value &val, osc::OutboundPacketStream *message) {
    // *message << osc::BeginBundleImmediate << osc::BeginMessage(url) << val << osc::EndMessage << osc::EndBundle;
    *message << osc::BeginMessage(url) << val << osc::EndMessage;
  }
  /** Access to OscCommand.
   */
  OscCommand *command_;

  /** Socket listening to udp packets.
   */
  UdpListeningReceiveSocket *socket_;

  std::list<IpEndpointName> observers_; /**< List of satellites that have registered to get return values back. */

  char osc_buffer_[OSC_OUT_BUFFER_SIZE];     /** Buffer used to build osc packets. */
  bool running_;
};


OscCommand::OscCommand(uint16_t port) :
                    Command("osc", OSCIT_SERVICE_TYPE, port) {
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

void OscCommand::notify_observers(const char *url, const Value &val) {
  send_to_observers(url, val);
}

void OscCommand::send_to_observers(const char *url, const Value &val, const IpEndpointName *skip_end_point) {
  impl_->send_to_observers(url, val, skip_end_point);
}

void OscCommand::listen() {
  impl_->listen();
}

void OscCommand::send(const IpEndpointName &remote_endpoint, const std::string &url, const Value &val) {
  impl_->send(remote_endpoint, url.c_str(), val);
}

void OscCommand::send(const IpEndpointName &remote_endpoint, const char *url, const Value &val) {
  impl_->send(remote_endpoint, url, val);
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

void OscCommand::process_message(const IpEndpointName &remote_endpoint, const std::string &url, const Value &val) {
  Value res = root_->call(url, val, this);
  
  // send return
  impl_->send_reply(remote_endpoint, url, res);
}

void OscCommand::change_port(uint16_t port) {
  impl_->change_port(port);
  port_ = port;
}

} // oscit

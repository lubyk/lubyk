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

//#define DEBUG_OSC_COMMAND

static osc::OutboundPacketStream &operator<<(osc::OutboundPacketStream &out_stream, const Value &val) {
  size_t sz;
  switch (val.type()) {
    case REAL_VALUE:
      out_stream << val.r;
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

  Implementation(OscCommand *command, uint port) : command_(command), root_(NULL), socket_(NULL), port_(port), zeroconf_registration_(NULL), running_(false) {}

  virtual ~Implementation() {
    kill();
    if (zeroconf_registration_ != NULL) delete zeroconf_registration_;
    if (socket_ != NULL) delete socket_;
  }

  void kill() {
    if (running_) socket_->AsynchronousBreak();
    running_ = false;
  }

  void set_root(Root *root) {
    root_ = root;
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
    osc::OutboundPacketStream message( osc_buffer_, OSC_OUT_BUFFER_SIZE );
    build_message(url, val, &message);
    socket_->Connect(remote_endpoint);
    socket_->Send(message.Data(), message.Size());
  }

  /** Start listening for incoming messages (runs in its own thread). */
  void do_listen() {
    assert(root_);

    if (zeroconf_registration_ == NULL) {
      std::string name(root_->name());
      if (name == "") {
        name = "Generic oscit device";
      }
      zeroconf_registration_ = new ZeroConfRegistration(name.c_str(), OSCIT_SERVICE_TYPE, port_);
    }

    if (socket_ == NULL) {
      socket_ = new UdpListeningReceiveSocket( IpEndpointName( IpEndpointName::ANY_ADDRESS, port_ ), this );
    }
    running_ = true;
    socket_->Run();
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
      char host_ip[ IpEndpointName::ADDRESS_STRING_LENGTH ];
      // get host ip as string
      remote_endpoint.AddressAsString(host_ip);
      std::cout << url << " " << val << " (" << host_ip << ":" << remote_endpoint.port << ")" << std::endl;
#endif
      res = root_->call(url, val, NULL);  // NULL = external context
    }

    // send return
    send_reply(&remote_endpoint, url, res);
  }

  /** Send reply to caller and notify observers. */
  void send_reply(const IpEndpointName *remote_endpoint, const std::string &url, const Value &val) {
    if (val.is_nil()) return;

    if (val.is_error()) {
      if (remote_endpoint) send(*remote_endpoint, "/.error", val);
    } else {
      // reply to all

      // prepare reply
      Value res(url);
      res.push_back(val);

      if (remote_endpoint) {
        send(*remote_endpoint, "/.reply", res);
      }

      send_all("/.reply", res, remote_endpoint); // skip remote_endpoint
      root_->notify_observers("/.reply", res, command_);   // skip this command
    }
  }

  /** Build osc message and send it to all observers. */
  void send_all(const char *url, const Value &val, const IpEndpointName *skip_end_point = NULL) {
    std::list<IpEndpointName>::const_iterator it  = observers_.begin();
    std::list<IpEndpointName>::const_iterator end = observers_.end();

    osc::OutboundPacketStream message( osc_buffer_, OSC_OUT_BUFFER_SIZE );
    build_message(url, val, &message);

    while (it != end) {
      if (skip_end_point != NULL && *it == *skip_end_point) {
        // skip
      } else {
        socket_->Connect(*it++);
        socket_->Send(message.Data(), message.Size());
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
    *message << osc::BeginBundleImmediate << osc::BeginMessage(url) << val << osc::EndMessage << osc::EndBundle;
  }
  /** Access to OscCommand.
   */
  OscCommand *command_;

  /** Direct link to root without passing by OscCommand.
   */
  Root *root_;

  /** Socket listening to udp packets.
   */
  UdpListeningReceiveSocket *socket_;

  /** Connected port.
   */
  uint port_;

  /** Zeroconf registration thread.
   */
  ZeroConfRegistration *zeroconf_registration_;

  std::list<IpEndpointName> observers_; /**< List of satellites that have registered to get return values back. */

  char osc_buffer_[OSC_OUT_BUFFER_SIZE];     /** Buffer used to build osc packets. */
  bool running_;
};


OscCommand::OscCommand(uint port) : Command("osc") {
  impl_ = new OscCommand::Implementation(this, port);
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
  impl_->send_all(url, val);
}

void OscCommand::do_listen() {
  impl_->set_root(root_);
  impl_->do_listen();
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

} // oscit

#include "oscit/root.h"
// meta methods
#include "oscit/info_meta_method.h"
#include "oscit/type_meta_method.h"
#include "oscit/error_meta_method.h"
#include "ip/UdpSocket.h"

namespace oscit {

Root::~Root() {
  if (osc_in_) delete osc_in_;
  clear();
  root_ = NULL; // avoid call to unregister_object in ~BaseObject
}

void Root::init() {
  root_ = NULL;
  register_object(this);
  
  // build meta methods
  adopt(new InfoMetaMethod(".info"));
  adopt(new TypeMetaMethod(".type"));
  adopt(new ErrorMetaMethod(".error"));
}


void Root::register_observer(const IpEndpointName &observer) {
  // TODO: check that the observer is not already in the list of observers_.
  observers_.push_back(observer);
}

void Root::send_reply(UdpSocket *socket, const IpEndpointName *remote_endpoint, const std::string &url, const Value &val) {
  UdpSocket * send_socket = socket;
  
  if (val.is_nil()) return;
  
  if (send_socket == NULL) {
    if (osc_in_) { 
      send_socket = osc_in_->socket();
    } else {
      fprintf(stderr, "cannot send_reply (no socket and no receive port open).\n");
      return;
    }
  }
  
  if (val.is_error()) {
    if (remote_endpoint) OscSend::send(send_socket, *remote_endpoint, "/.error", val);
  } else {
    // prepare reply depending on value
    Value res(url);
    res.push_back(val);
    
    if (remote_endpoint) {
      // TODO: check that the sender is not in the list of observers_.
      OscSend::send(send_socket, *remote_endpoint, "/.reply", res);
    }
    
    OscSend::send_all(send_socket, observers_, "/.reply", res);
  }
}

void Root::send(const IpEndpointName &remote_endpoint, const char *url, const Value &val) {
  UdpSocket * send_socket;
  if (osc_in_ == NULL) {
    fprintf(stderr, "cannot send (socket not opened). Use open_port first.\n");
    return;
  } else {
    send_socket = osc_in_->socket();
  }
  OscSend::send(send_socket, remote_endpoint, url, val);
}

} // namespace oscit
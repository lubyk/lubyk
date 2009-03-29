#include "oscit/root.h"
// meta methods
#include "oscit/meta_methods/info_meta_method.h"
#include "oscit/meta_methods/type_meta_method.h"
#include "ip/UdpSocket.h"

namespace oscit {
Root::~Root() {
  std::list<OscSend*>::iterator it  = observers_.begin();
  std::list<OscSend*>::iterator end = observers_.end();

  while (it != end) delete *it++;
  
  if (osc_in_) delete osc_in_;
  clear();
  root_ = NULL; // avoid call to unregister_object in ~Object
}

// build meta methods
void Root::build_meta_methods() {
  adopt(new InfoMetaMethod(".info"));
  adopt(new TypeMetaMethod(".type"));
}

void Root::send_reply(const IpEndpointName *remoteEndpoint, const std::string &url, const Value &val) {
  
  if (val.is_error()) {
    if (remoteEndpoint) OscSend(*remoteEndpoint).send("/.error", val);
  } else {
    // prepare reply depending on value
    Value res(url);
    res.push_back(val);
    
    if (remoteEndpoint) {
      // TODO: check that the sender is not in the list of observers_.
      OscSend(*remoteEndpoint).send("/.reply", res);
    }
    OscSend::send_all(observers_, "/.reply", res);
  }
}
} // namespace oscit
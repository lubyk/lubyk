// #include "oscit/osc_root_proxy.h"
// #include "oscit/osc_command.h"
// 
// oscit {
// 
// OscRootProxy::OscRootProxy(OscCommand *command, const IpEndpointName *remote_endpoint) : command_(command) {
//   remote_endpoint_ = new IpEndpointName(*remote_endpoint);
// }
// 
// OscRootProxy::~OscRootProxy() {
//   delete remote_endpoint_;
// }
// 
// 
// virtual void OscRootProxy::send_to_remote(const std::string &url, const Value &val); {
//   command_->send(remote_end_point_, url, val);
// }
// 
// } // oscit
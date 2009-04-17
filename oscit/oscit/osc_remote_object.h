#ifndef _OSCIT_OSC_REMOTE_OBJECT_H_
#define _OSCIT_OSC_REMOTE_OBJECT_H_
#include "oscit/object.h"

#include "ip/UdpSocket.h"

namespace oscit {

class OscRemoteObject : public Object
{
  OscRemoteObject(OscCommand *osc_command, const IpEndpointName &endpoint, const std::string &remote_path) : 
      osc_command_(osc_command), remote_endpoint_(endpoint), remote_path_(remote_path), connected_(false) {
    connect();
  }
  
  virtual const Value trigger(const Value &val) {
    osc_command_->send(remote_endpoint_, remote_path_, val);
    return gNilValue;
  }
  
  
  void connect() {
    // try to connect ? async ...
  }
  
private:
  OscCommand    *osc_command_;
  IpEndpointName remote_endpoint_;
  std::string    remote_path_;
  bool           connected_;
};

} // namespace osc

#endif // _OSCIT_OSC_REMOTE_OBJECT_H_
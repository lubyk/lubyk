#ifndef OSCIT_INCLUDE_OSCIT_OSC_REMOTE_OBJECT_H_
#define OSCIT_INCLUDE_OSCIT_OSC_REMOTE_OBJECT_H_
#include "oscit/object.h"

namespace oscit {

// FIXME: I think we should find a way to replace this by ObjectProxy..

class OscRemoteObject : public Object
{
 public:
  /** Class signature. */
  TYPED("Object.OscRemoteObject")

  OscRemoteObject(OscCommand *osc_command, const Location &endpoint, const std::string &remote_path) :
      osc_command_(osc_command), remote_endpoint_(endpoint), remote_path_(remote_path), connected_(false) {
    connect();
  }

  virtual const Value trigger(const Value &val) {
    osc_command_->send(remote_endpoint_, remote_path_.c_str(), val);
    return gNilValue;
  }


  void connect() {
    // try to connect ? async ...
  }

private:
  OscCommand    *osc_command_;
  Location       remote_endpoint_;
  std::string    remote_path_;
  bool           connected_;
};

} // namespace osc

#endif // OSCIT_INCLUDE_OSCIT_OSC_REMOTE_OBJECT_H_

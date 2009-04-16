#include "oscit/base_command.h"
#include "oscit/root.h"
#include "oscit/base_object.h"

namespace oscit {

BaseCommand::~BaseCommand() {
  kill();
  if (root_) {
    root_->unregister_command(this);
    root_ = NULL;
  }
  string_iterator it;
  string_iterator end = remote_objects_.end();

  // destroy all references to remote objects
  for(it = remote_objects_.begin(); it != end; it++) {
    BaseObject * object;
    if (remote_objects_.get(*it, &object)) {
      delete object;
    }
  }
}

BaseObject *BaseCommand::remote_object(const Url &remote_url, Value *error) {
  BaseObject *object;
  if (remote_objects_.get(remote_url.str(), &object)) {
    return object;
  } else {
    return build_remote_object(remote_url, error);
  }
}

} // oscit
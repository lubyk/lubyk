#include "oscit/command.h"
#include "oscit/root.h"
#include "oscit/object.h"

namespace oscit {

Command::~Command() {
  kill();
  if (root_) {
    root_->unregister_command(this);
    root_ = NULL;
  }
  string_iterator it;
  string_iterator end = remote_objects_.end();

  // destroy all references to remote objects
  for(it = remote_objects_.begin(); it != end; it++) {
    Object * object;
    if (remote_objects_.get(*it, &object)) {
      delete object;
    }
  }
}

Object *Command::remote_object(const Url &remote_url, Value *error) {
  Object *object;
  if (remote_objects_.get(remote_url.str(), &object)) {
    return object;
  } else {
    return build_remote_object(remote_url, error);
  }
}

} // oscit

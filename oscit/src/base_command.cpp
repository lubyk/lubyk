#include "oscit/base_command.h"
#include "oscit/root.h"
#include "oscit/base_object.h"

namespace oscit {
BaseCommand::BaseCommand(const char *protocol) : root_(NULL), protocol_(protocol) {
  remote_objects_ = new Root;
}

BaseCommand::BaseCommand(const std::string &protocol) : root_(NULL), protocol_(protocol) {}

BaseCommand::BaseCommand(Root *root, const char *protocol) : root_(root), protocol_(protocol) {}

BaseCommand::BaseCommand(Root *root, const std::string &protocol) : root_(root), protocol_(protocol) {}

BaseCommand::~BaseCommand() {
  kill();
  if (root_) {
    root_->unregister_command(this);
    root_ = NULL;
  }
  delete remote_objects_;
}
BaseObject *BaseCommand::remote_object(const Url &remote_url) {
  BaseObject *object = remote_objects_->object_at(remote_url.str());
  return object ? object : build_remote_object(remote_url);
}

} // oscit
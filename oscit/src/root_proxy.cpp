#include "oscit/root_proxy.h"
// meta methods
#include "oscit/command.h"

namespace oscit {

void RootProxy::set_command(Command* command) {
  if (command_) command_->unregister_proxy(this);
  command_ = command;
  if (command_) command_->register_proxy(this);
}

} // oscit

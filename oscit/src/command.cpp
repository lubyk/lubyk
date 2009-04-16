#include "oscit/base_command.h"
#include "oscit/root.h"

namespace oscit {

Command::~Command() {
  if (root_) {
    root_->unregister_command(this);
    root_ = NULL;
  }
}

} // oscit
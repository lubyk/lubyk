#include "oscit/callback.h"

#include "assert.h"

#include <list>

#include "oscit/callback_list.h"
#include "oscit/on_destroy_notifier.h"
#include "oscit/callback_observer.h"

namespace oscit {

Callback::~Callback() {
  if (observer_)
    observer_->callback_destroyed(callback_list_, this);
}

void Callback::set_list(CallbackList *list) {
  assert(!callback_list_);
  callback_list_ = list;
  observer_->callback_produced(list, this);
}

} // oscit
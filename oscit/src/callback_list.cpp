#include "oscit/callback_list.h"

#include <iostream>

#include "oscit/observer.h"

namespace oscit {

void CallbackList::delete_callback(Callback *callback) {
  list_owner_->observer_lock();
    Iterator it = callbacks_.begin(), end = callbacks_.end();
    while (it != end) {
      if (*it == callback) {
        delete *it;
        callbacks_.erase(it);
        break;
      } else {
        ++it;
      }
    }
  list_owner_->observer_unlock();
}

} // oscit

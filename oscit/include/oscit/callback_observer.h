#ifndef OSCIT_INCLUDE_OSCIT_CALLBACK_OBSERVER_H_
#define OSCIT_INCLUDE_OSCIT_CALLBACK_OBSERVER_H_

#include "oscit/callback_list.h"

namespace oscit {

class Callback;

/** In order to create callbacks, an observer must inherit from CallbackObserver
 * so that it has a list of callbacks to disable on destruction.
 */
class CallbackObserver {
public:
  CallbackObserver() {}

  virtual ~CallbackObserver() {
    std::list<CallbackWithList*>::iterator it, end = produced_callbacks_.end();
    for (it = produced_callbacks_.begin(); it != end; ++it) {
      CallbackWithList *cl = *it;
      // we are dying, remove all callbacks that we have registered
      cl->callback_->observer_ = NULL;
      cl->list_->remove_callback(cl->callback_);
      delete cl;
    }
  }

private:
  friend class Callback;

  struct CallbackWithList {
    CallbackWithList(CallbackList *list, Callback *callback) : list_(list), callback_(callback) {}
    CallbackList *list_;
    Callback *callback_;
  };

  void callback_produced(CallbackList *list, Callback *callback) {
    produced_callbacks_.push_back(new CallbackWithList(list, callback));
  }

  void callback_destroyed(CallbackList *list, Callback *callback) {
    std::list<CallbackWithList*>::iterator it = produced_callbacks_.begin(), end = produced_callbacks_.end();
    while (it != end) {
      CallbackWithList *cl = *it;
      if (cl->list_ == list && cl->callback_ == callback) {
        delete cl;
        it = produced_callbacks_.erase(it);
      } else {
        ++it;
      }
    }
  }

  std::list<CallbackWithList*> produced_callbacks_;
};
} // oscit

#endif // OSCIT_INCLUDE_OSCIT_CALLBACK_OBSERVER_H_
#ifndef OSCIT_INCLUDE_OSCIT_OBSERVER_H_
#define OSCIT_INCLUDE_OSCIT_OBSERVER_H_

#include "oscit/observable.h"

namespace oscit {

class Observer {
public:

  /** Set an instance variable to some target value and register the necessary
   *  callbacks to invalidate (set to NULL) the instance variable in case the
   *  target is deleted.
   */
  template<class T>
  void set_and_hold(T **ivar_ptr, T *target) {
    if (*ivar_ptr) {
      // stop observing previous value
      destruction_notifier_.trigger_and_remove(ivar_ptr);
    }
    // set
    *ivar_ptr = target;
    // register notification if address becomes invalid
    target->on_destroy(new Observable::TCallback<Observer, &Observer::clear_ivar>(this, ivar_ptr));
    // if we die before the target, remove us from the notification list
    destruction_notifier_.adopt_callback(
      new Observable::TCallback<Observable, &Observable::disable_on_destroy>(target, ivar_ptr)
    );
  }

protected:
  /** If you are in a multi-threaded environment, you should overwrite this method.
   * to lock before notification trigger.
   */
  virtual void observer_lock() {}

  /** If you are in a multi-threaded environment, you should overwrite this method.
   * to unlock after notification trigger.
   */
  virtual void observer_unlock() {}

private:

  /** This method invalidates an instance variable.
   * It is called by an observed (Observable class) object on
   * destruction.
   */
  void clear_ivar(void *ivar_ptr) {
    void **ivar = (void **)ivar_ptr;
    observer_lock();
      *ivar = NULL;
      destruction_notifier_.remove_callback(ivar_ptr);
    observer_unlock();
  }

  Observable::OnDestroyNotifier destruction_notifier_;
};

} // oscit
#endif // OSCIT_INCLUDE_OSCIT_OBSERVER_H_
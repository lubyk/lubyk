#ifndef OSCIT_INCLUDE_OSCIT_CALLBACK_H_
#define OSCIT_INCLUDE_OSCIT_CALLBACK_H_

namespace oscit {

#ifndef NULL
#define NULL 0
#endif

class CallbackList;
class CallbackObserver;

/** The Callback class holds a pointer to the target of the callback
 * and a method (eventually with parameters) to call the target back.
 *
 * The class should  be used in conjonction with the CallbackList so
 * that the Callback is removed from the CallbackList if it's target
 * pointer becomes invalid.
 */
class Callback {
public:
  Callback(CallbackObserver *observer) : observer_(observer), callback_list_(NULL) {}

  virtual ~Callback();

  /** The callback has been inserted in a CallbackList, we can install
   * the on_destroy notification in the observer.
   */
  void set_list(CallbackList *list);

  CallbackObserver *observer() {
    return observer_;
  }

  virtual void trigger() = 0;
protected:
  friend class CallbackObserver;

  /** Pointer to the observer so we can reach it's on_destroy notification
   * list. The observer's on_destroy list is used to invalidate registered
   * callback for the observer. If this callback is destroyed before the
   * observer, we must remove it from the list.
   */
  CallbackObserver *observer_;

private:
  /** The CallbackList containing this Callback. We need a pointer to the
   * list in order to remove the on_destroy callback in the observer: this
   * callback's target is callback_list_->remove_callback(this).
   */
  CallbackList *callback_list_;
};

template<class T, void(T::*Tmethod)(void *data)>
class TCallback : public Callback {
public:
  TCallback(T *observer, void *data) : Callback(observer), data_(data) {}

  ~TCallback() {}
  virtual void trigger() {
    (((T*)observer_)->*Tmethod)(data_);
  }

private:
  void *data_;
};

} // oscit

#endif // OSCIT_INCLUDE_OSCIT_CALLBACK_H_
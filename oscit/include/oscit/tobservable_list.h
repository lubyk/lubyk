#ifndef OSCIT_INCLUDE_OSCIT_TOBSERVABLE_LIST_H_
#define OSCIT_INCLUDE_OSCIT_TOBSERVABLE_LIST_H_

#include <list>

#include "oscit/callback.h"
#include "oscit/observer.h"

namespace oscit {

/** Use this instead of a std::list to hold pointers to Observable
 * instances if these instances might become invalid before the
 * owner of the list.
 *
 * TODO: make this a sub-class of std::list ?
 */
template<class T>
class TObservableList : public Observer {
public:
  TObservableList(Observer *list_owner) : list_owner_(list_owner) {}

  void push_back(T target) {
    list_.push_back(target);

    // register notification if address becomes invalid
    target->adopt_callback_on_destroy(
      new TCallback<TObservableList, &TObservableList::remove_from_list>(this, target)
    );
  }

  void remove(T target) {
    // remove remote "on_destroy" callback
    delete_produced_callbacks_with_data(target);
    list_.remove(target);
  }

  T front() {
    return list_.front();
  }

  size_t size() {
    return list_.size();
  }
protected:
  /** The target is becoming invalid, remove from list (called from the target's
   * "on_destroy" list).
   */
  void remove_from_list(void *data) {
    T target = (T)data;
    // stop observing this target
    list_owner_->observer_lock();
      list_.remove(target);
    list_owner_->observer_unlock();
  }

  virtual void observer_lock() {
    list_owner_->observer_lock();
  }

  virtual void observer_unlock() {
    list_owner_->observer_unlock();
  }

private:
  Observer *list_owner_;
  std::list<T> list_;
};


} // oscit

#endif // OSCIT_INCLUDE_OSCIT_TOBSERVABLE_LIST_H_
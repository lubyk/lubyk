/*
  ==============================================================================

   This file is part of the OSCIT library (http://rubyk.org/liboscit)
   Copyright (c) 2007-2009 by Gaspard Bucher - Buma (http://teti.ch).

  ------------------------------------------------------------------------------

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

  ==============================================================================
*/

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

  typename std::list<T>::iterator begin() {
    return list_.begin();
  }

  typename std::list<T>::iterator end() {
    return list_.end();
  }

  typename std::list<T>::const_iterator begin() const {
    return list_.begin();
  }

  typename std::list<T>::const_iterator end() const {
    return list_.end();
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
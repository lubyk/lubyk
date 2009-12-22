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

#ifndef OSCIT_INCLUDE_OSCIT_OBSERVER_H_
#define OSCIT_INCLUDE_OSCIT_OBSERVER_H_

#include <list>
#include <iostream>

#include "oscit/callback_list.h"

namespace oscit {

class Callback;

/** In order to create callbacks, an observer must inherit from Observer
 * so that it has a list of callbacks to disable on destruction.
 */
class Observer {
public:

  virtual ~Observer() {
    std::list<CallbackWithList*>::iterator it, end = produced_callbacks_.end();
    for (it = produced_callbacks_.begin(); it != end; ++it) {
      CallbackWithList *cl = *it;
      // we are dying, remove all callbacks that we have registered
      cl->callback_->observer_ = NULL;
      cl->list_->delete_callback(cl->callback_);
      delete cl;
    }
  }

  /** Set an instance variable to some target value and register the necessary
   *  callbacks to invalidate (set to NULL) the instance variable in case the
   *  target is deleted.
   */
  template<class T>
  void set_and_hold(T **ivar_ptr, T *target) {
    if (*ivar_ptr) {
      // stop observing previous value
      delete_produced_callbacks_with_data(ivar_ptr);
    }

    // set
    *ivar_ptr = target;

    // register notification if address becomes invalid
    target->adopt_callback_on_destroy(
      new TCallback<Observer, &Observer::clear_ivar>(this, ivar_ptr)
    );
  }

  /** If you are in a multi-threaded environment, you should overwrite this method.
   * to lock before notification trigger.
   */
  virtual void observer_lock() = 0;

  /** If you are in a multi-threaded environment, you should overwrite this method.
   * to unlock after notification trigger.
   */
  virtual void observer_unlock() = 0;

protected:
  void delete_produced_callbacks_with_data(void *data) {
    std::list<CallbackWithList*>::iterator it = produced_callbacks_.begin(), end = produced_callbacks_.end();
    while (it != end) {
      CallbackWithList *cl = *it;
      if (cl->callback_->match_data(data)) {
        // found matching callback: remove
        cl->list_->delete_callback(cl->callback_);
        delete cl;
        it = produced_callbacks_.erase(it);
      } else {
        ++it;
      }
    }
  }

private:
  friend class Callback;

  struct CallbackWithList {
    CallbackWithList(CallbackList *list, Callback *callback) : list_(list), callback_(callback) {}
    CallbackList *list_;
    Callback *callback_;
  };

  /** This method invalidates an instance variable.
   * It is called by an observed (Observable class) object on
   * destruction.
   */
  void clear_ivar(void *ivar_ptr) {
    void **ivar = (void **)ivar_ptr;
    observer_lock();
      *ivar = NULL;
    observer_unlock();
  }

  /** This method is called when a callback is inserted in
   * a CallbackList.
   */
  void callback_produced(CallbackList *list, Callback *callback) {
    produced_callbacks_.push_back(new CallbackWithList(list, callback));
  }

  /** This method is called when a produced callback is being
   * destroyed.
   */
  void callback_destroyed(CallbackList *list, Callback *callback) {
    observer_lock();
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
    observer_unlock();
  }

  std::list<CallbackWithList*> produced_callbacks_;
};

} // oscit
#endif // OSCIT_INCLUDE_OSCIT_OBSERVER_H_
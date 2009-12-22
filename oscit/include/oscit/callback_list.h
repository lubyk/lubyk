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

#ifndef OSCIT_INCLUDE_OSCIT_CALLBACK_LIST_H_
#define OSCIT_INCLUDE_OSCIT_CALLBACK_LIST_H_
#include <list>

#include "oscit/callback.h"
#include "oscit/non_copyable.h"

namespace oscit {

class Observer;

/** Holds a list of callbacks. The list is responsible for staying
 * sane (no callbacks to dangling pointers) by registering its own
 * callback in the original callback target to be notified on destroy.
 */
class CallbackList : private NonCopyable {
public:
  CallbackList(Observer *list_owner) : list_owner_(list_owner) {}

  typedef std::list<Callback*>::iterator Iterator;

  virtual ~CallbackList() {
    Iterator it, end = callbacks_.end();
    for(it = callbacks_.begin(); it != end; ++it) {
      delete *it;
    }
  }

  /** Adopt a new callback.
   *  The notifier becomes responsible for the callback deletion.
   */
  void adopt_callback(Callback *callback) {
    callbacks_.push_back(callback);
    callback->set_list(this);
  }

  /** Remove a specific callback from the list.
   */
  void delete_callback(Callback *callback);

  // /** Remove a specific callback from the list.
  //  */
  // void remove_callbacks_to(Observer *observer) {
  //   Iterator it = callbacks_.begin(), end = callbacks_.end();
  //   while (it != end) {
  //     if ((*it)->observer() == observer) {
  //       delete *it;
  //       it = callbacks_.erase(it);
  //     } else {
  //       ++it;
  //     }
  //   }
  // }

  /** Trigger all callbacks in the list.
   */
  void trigger_all() {
    Iterator it, end = callbacks_.end();
    for(it = callbacks_.begin(); it != end; ++it) {
      (*it)->trigger();
    }
  }
  /** Find the callback identified by the given key,
   *  trigger it and remove from list.
   */
  // void trigger_and_remove(void *var, void *data) {
  //   std::list<Callback*>::iterator it, end;
  //   end = callbacks_.end();
  //   for (it = callbacks_.begin(); it != end; ++it) {
  //     if ((*it)->key_match(var, data)) {
  //       (*it)->trigger();
  //       delete *it;
  //       it = callbacks_.erase(it);
  //     }
  //   }
  // }

protected:
  Observer *list_owner_;
  std::list<Callback*> callbacks_;
};


} // oscit

#endif // OSCIT_INCLUDE_OSCIT_CALLBACK_LIST_H_
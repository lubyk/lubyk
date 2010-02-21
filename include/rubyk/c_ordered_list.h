/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
   Copyright (c) 2007-2010 by Gaspard Bucher - Buma (http://teti.ch).

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

#ifndef RUBYK_INCLUDE_RUBYK_C_ORDERED_LIST_H_
#define RUBYK_INCLUDE_RUBYK_C_ORDERED_LIST_H_

#include "rubyk/oscit.h"
#include "rubyk/ordered_list.h"

/** The Concurrent OrderedList template is a thread-safe OrderedList for
 * push, get, get_and_pop and pop operations. If you need to use an iterator
 * or other methods, you have to lock.
 */
template <class T>
class COrderedList : public OrderedList<T>, public Mutex {
public:
  void push(T object) {
    ScopedLock lock(mutex_);
    OrderedList<T>::push(object);
  }

  bool get_and_pop(T *object) {
    ScopedLock lock(mutex_);
    if (this->empty()) {
      return false;
    } else {
      *object = this->linked_list_->next->obj;
      OrderedList<T>::pop();
      return true;
    }
  }

  bool get(T *object) {
    ScopedLock lock(this);
    return OrderedList<T>::get(object);
  }

  void pop() {
    ScopedLock lock(this);
    OrderedList<T>::pop();
  }
};

template <class T>
class COrderedList<T*> : public OrderedList<T*>, public Mutex {
public:
  void push(T *object) {
    ScopedLock lock(this);
    OrderedList<T*>::push(object);
  }

  bool get_and_pop(T **object) {
    ScopedLock lock(this);
    if (this->empty()) {
      return false;
    } else {
      *object = this->linked_list_->next->obj;
      OrderedList<T*>::pop();
      return true;
    }
  }

  bool get(T **object) {
    ScopedLock lock(this);
    return OrderedList<T*>::get(object);
  }

  void pop() {
    ScopedLock lock(this);
    OrderedList<T*>::pop();
  }
};

#endif // RUBYK_INCLUDE_RUBYK_C_ORDERED_LIST_H_
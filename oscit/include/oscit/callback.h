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

#ifndef OSCIT_INCLUDE_OSCIT_CALLBACK_H_
#define OSCIT_INCLUDE_OSCIT_CALLBACK_H_

namespace oscit {

#ifndef NULL
#define NULL 0
#endif

class CallbackList;
class Observer;

/** The Callback class holds a pointer to the target of the callback
 * and a method (eventually with parameters) to call the target back.
 *
 * The class should  be used in conjonction with the CallbackList so
 * that the Callback is removed from the CallbackList if it's target
 * pointer becomes invalid.
 */
class Callback {
public:
  Callback(Observer *observer, void *data)
      : observer_(observer),
        data_(data),
        callback_list_(NULL) {}

  virtual ~Callback();

  /** The callback has been inserted in a CallbackList, we can install
   * the on_destroy notification in the observer.
   */
  void set_list(CallbackList *list);

  Observer *observer() {
    return observer_;
  }

  virtual void trigger() = 0;

  bool match_data(void *data) {
    return data_ == data;
  }
protected:
  friend class Observer;

  /** Pointer to the observer so we can reach it's on_destroy notification
   * list. The observer's on_destroy list is used to invalidate registered
   * callback for the observer. If this callback is destroyed before the
   * observer, we must remove it from the list.
   */
  Observer *observer_;

  /** Any data that will be passed back to the caller.
   */
   void *data_;

private:
  /** The CallbackList containing this Callback. We need a pointer to the
   * list in order to remove the on_destroy callback reference in the observer.
   */
  CallbackList *callback_list_;
};

template<class T, void(T::*Tmethod)(void *data)>
class TCallback : public Callback {
public:
  TCallback(T *observer, void *data) : Callback(observer, data) {}

  ~TCallback() {}

  virtual void trigger() {
    (((T*)observer_)->*Tmethod)(data_);
  }
};

} // oscit

#endif // OSCIT_INCLUDE_OSCIT_CALLBACK_H_
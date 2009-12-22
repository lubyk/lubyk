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

#ifndef OSCIT_INCLUDE_OSCIT_OBSERVABLE_H_
#define OSCIT_INCLUDE_OSCIT_OBSERVABLE_H_

#include <list>

#include "oscit/observer.h"
#include "oscit/on_destroy_notifier.h"

namespace oscit {

class Callback;

/** This class should be used with the Observer class. It holds a list of callbacks to trigger on
 * destruction (to invalidate the references pointing to instances of this class).
 */
class Observable : public Observer {
public:
  Observable() : destruction_notifier_(this) {}

  void adopt_callback_on_destroy(Callback *callback) {
    destruction_notifier_.adopt_callback(callback);
  }

private:
  /** List of callbacks to trigger on destruction.
   */
  OnDestroyNotifier destruction_notifier_;
};

} // oscit
#endif // OSCIT_INCLUDE_OSCIT_OBSERVABLE_H_
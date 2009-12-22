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

#ifndef OSCIT_INCLUDE_OSCIT_CALL_H_
#define OSCIT_INCLUDE_OSCIT_CALL_H_
#include <string>
#include "oscit/root.h"

namespace oscit {

/** This is a simple struct to store a call (url and parameter). */
struct Call
{
  Call(const std::string &url, const Value &param) : url_(url), param_(param) {}
  Call(const char        *url, const Value &param) : url_(url), param_(param) {}
  Call(const Url         &url, const Value &param) : url_(url), param_(param) {}

  const Value safe_trigger(Root *root, Mutex *context) {
    return root->call(url_, param_, context);
  }

  Url url_;
  Value param_;
};
} // oscit

#endif // OSCIT_INCLUDE_OSCIT_CALL_H_

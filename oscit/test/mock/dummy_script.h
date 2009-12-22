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

#ifndef OSCIT_TEST_MOCK_DUMMY_SCRIPT_H_
#define OSCIT_TEST_MOCK_DUMMY_SCRIPT_H_
#include <string>

#include "oscit/script.h"

class DummyScript : public Script {
public:
  DummyScript() {}
  
  virtual const Value eval_script() {
    size_t pos = script_.find("compilation error");
    if (pos != std::string::npos) {
      return Value(BAD_REQUEST_ERROR, std::string("Compilation error near '").append(script_.substr(pos - 4, 23)).append("'."));
    }
    return Value(script_);
  }
  
  void reload_script(time_t current_time) {
    this->Script::reload_script(current_time);
  }
};

#endif // OSCIT_TEST_MOCK_DUMMY_SCRIPT_H_
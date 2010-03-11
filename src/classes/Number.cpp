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

#include "rubyk.h"

class NumberNode : public Node
{
public:
  NumberNode() {
    printf("[%p] NumberNode\n", this);
  }

  virtual ~NumberNode() {
    printf("[%p] ~NumberNode\n", this);
  }

  // [1] set/get value, send value
  const Value value(const Value &val) {
    printf("[%p] NumberNode::value\n", this);
    if (!val.is_nil()) {
      value_ = val;
    }
    send(value_);
    return value_;
  }

  virtual void inspect(Value *hash) const {
    hash->set("number", value_);
  }

private:
  Value value_;
};

extern "C" void init(Planet &planet) {
  CLASS_NAMED(NumberNode, "Number", "Stores a number which can be sent again through Bang!.", "value: [initial value]");
  METHOD(NumberNode, value, RealIO("Current number."))
  OUTLET(NumberNode, value, RealIO("Send the current number out."))
}
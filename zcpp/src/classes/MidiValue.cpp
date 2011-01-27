/*
  ==============================================================================

   This file is part of the LUBYK project (http://lubyk.org)
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

#include "lubyk.h"
using namespace rk;

class MidiValueNode : public Node {
public:
  virtual const Value init() {
    Node::init();

    value_.set(MidiMessage());

    return gNilValue;
  }

  /** [1] set/get value, send value.
   */
  const Value value(const Value &val) {
    if (val.is_nil()) return value_;

    if (!val.is_bang()) {
      value_ = val;
    }

    send(value_);
    return value_;
  }

  /** [2] set/get value without sending.
   */
  const Value set_value(const Value &val) {
    if (val.is_midi()) value_ = val;

    return value_;
  }

  virtual void inspect(Value *hash) const {
    hash->set("value", value_.midi_message_->to_s());
  }

  /** Default key to use when creating an object without a hash definition.
   * This means that Value(34) is the same as Value(value:34).
   */
  virtual const char *default_set_key() const {
    return "value";
  }

private:
  Value value_;
};

extern "C" void init(Planet &planet) {
  CLASS_NAMED(MidiValueNode, "MidiValue", "Stores a value which can be sent again through Bang!.", "value: [initial midi value]")
  METHOD(MidiValueNode, value,     Oscit::midi_io("Set/get current midi value."))
  METHOD(MidiValueNode, set_value, Oscit::midi_io("Set/get current midi value (do not send)."))
  OUTLET(MidiValueNode, value,     Oscit::midi_io("Send the current midi value out."))
}
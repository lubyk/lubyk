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

#ifndef OSCIT_INCLUDE_OSCIT_OSC_MAP_COMMAND_H_
#define OSCIT_INCLUDE_OSCIT_OSC_MAP_COMMAND_H_

#include <list>
#include <stdexcept> // runtime_error

#include "oscit/osc_command.h"
#include "oscit/script.h"
#include "oscit/mapper.h"
#include "oscit/values.h"
#include "oscit/time_ref.h"

namespace oscit {

class OscMapCommand : public Script, public OscCommand {
public:
  OscMapCommand();

  OscMapCommand(uint16_t port, uint16_t reply_port);

  virtual ~OscMapCommand();

  // {4} set/change input port
  const Value port(const Value &val) {
    if (val.is_real() && val.r > 0 && val.r != port_) {
      lock();
        change_port(val.r);
      unlock();
    }

    return Value(port_);
  }

  // {5} set/change input port
  const Value reply_port(const Value &val) {
    if (val.is_real() && val.r > 0) {
      lock();
        reply_port_ = val.r;
      unlock();
    }

    return Value(reply_port_);
  }

 protected:
  /** Executed within mutex lock from own thread.
   */
  virtual void receive(const Url &url, const Value &ext_val);

  /** Executed within mutex lock from root.
   */
  virtual void notify_observers(const char *path, const Value &val);

  /** Mappings compilation.
   */
  virtual const Value eval_script() {
    Value res;
    lock();
      if (mapper_.parse(script_.c_str())) {
        res = Value(script_);
      } else {
        res = Value(BAD_REQUEST_ERROR, mapper_.error());
      }
    unlock();
    return res;
  }

 private:

  uint16_t reply_port_;

  Mapper mapper_;
  TimeRef time_ref_;
};

}  // oscit

#endif  // OSCIT_INCLUDE_OSCIT_OSC_MAP_COMMAND_H_
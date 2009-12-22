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

#ifndef OSCIT_INCLUDE_OSCIT_OSC_REMOTE_OBJECT_H_
#define OSCIT_INCLUDE_OSCIT_OSC_REMOTE_OBJECT_H_
#include "oscit/object.h"

namespace oscit {

// FIXME: I think we should find a way to replace this by ObjectProxy..

class OscRemoteObject : public Object
{
 public:
  /** Class signature. */
  TYPED("Object.OscRemoteObject")

  OscRemoteObject(OscCommand *osc_command, const Location &endpoint, const std::string &remote_path) :
      osc_command_(osc_command), remote_endpoint_(endpoint), remote_path_(remote_path), connected_(false) {
    connect();
  }

  virtual const Value trigger(const Value &val) {
    osc_command_->send(remote_endpoint_, remote_path_.c_str(), val);
    return gNilValue;
  }


  void connect() {
    // try to connect ? async ...
  }

private:
  OscCommand    *osc_command_;
  Location       remote_endpoint_;
  std::string    remote_path_;
  bool           connected_;
};

} // namespace osc

#endif // OSCIT_INCLUDE_OSCIT_OSC_REMOTE_OBJECT_H_

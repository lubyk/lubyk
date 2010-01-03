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

#ifndef OSCIT_INCLUDE_OSCIT_OSC_COMMAND_H_
#define OSCIT_INCLUDE_OSCIT_OSC_COMMAND_H_
#include <list>

#include "oscit/command.h"

namespace oscit {

class Location;
class Root;

class OscCommand : public Command {
public:
  TYPED("Mutex.Thread.Command.OscCommand")

  OscCommand(uint16_t port);

  OscCommand(const char *protocol, const char *service_type = "", uint16_t port = 0);

  virtual ~OscCommand();

  virtual void kill();

  virtual void notify_observers(const char *path, const Value &val);

protected:
  /** Create a reference to a remote object. */
  virtual Object *build_remote_object(const Url &url, Value *error);

  /** Send a Value to a remote endpoint at a given path using osc.
   */
  virtual void send_message(const Location &remote_endpoint, const char *path, const Value &val);

  /** Start listening for incoming messages (runs in its own thread).
   */
  virtual void listen();

  /** Listen to messages on a different port number.
   */
  void change_port(uint16_t port);

private:
  class Implementation;
  Implementation *impl_;
};

} // oscit

#endif // OSCIT_INCLUDE_OSCIT_OSC_COMMAND_H_

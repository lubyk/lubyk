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

#ifndef OSCIT_TEST_MOCK_OSC_COMMAND_LOGGER_H_
#define OSCIT_TEST_MOCK_OSC_COMMAND_LOGGER_H_

#include "oscit/command.h"
#include "mock/logger.h"

class OscCommandLogger : public OscCommand, protected MockLogger {
public:
  OscCommandLogger(uint port, const char *name, std::ostringstream *stream) :
                     OscCommand(port),
                     MockLogger(name, stream) {}


  virtual void listen() {
    log("listen");
    OscCommand::listen();
  }

  virtual void notify_observers(const char *path, const Value &val) {
    log("notify_observers", path, val);
    OscCommand::notify_observers(path, val);
  }

  virtual void send_message(const Location &remote_endpoint, const char *path, const Value &val) {
    log("send", remote_endpoint, path, val);
    OscCommand::send_message(remote_endpoint, path, val);
  }

  virtual void receive(const Url &url, const Value &val) {
    log("receive", url, val);
    replies_ << val << "\n";
    Command::receive(url, val);
  }

  void clear_replies() {
    replies_.str("");
  }

  const std::string replies() {
    return replies_.str();
  }

private:
  Logger replies_;
};


#endif // OSCIT_TEST_MOCK_OSC_COMMAND_LOGGER_H_
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

#ifndef OSCIT_TEST_MOCK_COMMAND_LOGGER_H_
#define OSCIT_TEST_MOCK_COMMAND_LOGGER_H_

#include "oscit/command.h"
#include "mock/logger.h"
#include "mock/dummy_object.h"

class CommandLogger : public Command, protected MockLogger {
public:
  CommandLogger(std::ostringstream *stream) :
                     Command("dummy"),
                     MockLogger("dummy", stream),
                     dummy_host_("dummy", "dummy.host", 2009) {}
  CommandLogger(const char *protocol, std::ostringstream *stream) :
                     Command(protocol),
                     MockLogger(protocol, stream),
                     dummy_host_("dummy", "dummy.host", 2009) {}
  CommandLogger(const char *name, const char *protocol, std::ostringstream *stream) :
                     Command(protocol),
                     MockLogger(name, stream),
                     dummy_host_("dummy", "dummy.host", 2009) {}


  ~CommandLogger() {
   // make sure command is halted before being destroyed
   kill();
  }

  virtual void listen() {
    log("listen");
    thread_ready();
    while (should_run()) {
      lock();
        log(".");
      unlock();
      millisleep(20);
    }
  }

  virtual void notify_observers(const char *path, const Value &val) {
    log("notify", path, val);
  }

  virtual void send_message(const Location &remote_endpoint, const char *path, const Value &val) {
    log("send", remote_endpoint, path, val);
  }

  virtual Object *build_remote_object(const Url &url, Value *error) {
    log("build_remote_object", url);
    if (url.location() == dummy_host_) {
      return adopt_remote_object(url.str(), new DummyObject(url.str().c_str(), url.port()));
    } else {
      error->set(BAD_REQUEST_ERROR, std::string("Unknown location '").append(url.location().name()).append("'."));
      return NULL;
    }
  }

  Object *remote_object_no_build(const std::string &url) {
    Object *res = NULL;
    remote_objects_.get(url, &res);
    return res;
  }

  const std::list<Location> &observers() const {
    return Command::observers();
  }

  Location dummy_host_;
};


#endif // OSCIT_TEST_MOCK_COMMAND_LOGGER_H_
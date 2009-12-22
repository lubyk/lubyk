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

#include "test_helper.h"

#include <sstream>

#include "ip/UdpSocket.h"

#include "oscit/zeroconf_registration.h"
#include "oscit/zeroconf_browser.h"
#include "oscit/proxy_factory.h"
#include "mock/logger.h"
#include "mock/command_logger.h"

class DummyBrowser : public ZeroConfBrowser {
public:
  DummyBrowser(const char *service) : ZeroConfBrowser(service), stream_(std::ostringstream::out), found_devices_(10) {}

  virtual void added_proxy(RootProxy *proxy) {
    stream_ << "[+ " << proxy->remote_location() << "]";
  }

  virtual void removed_proxy(RootProxy *proxy) {
    stream_ << "[- " << proxy->remote_location() << "]";
  }

  const std::string str() { return stream_.str(); }
  void str(const char *string) { stream_.str(string); }

private:
  std::ostringstream stream_;
  THash<std::string, bool> found_devices_;
};

class DummyRegistration : public ZeroConfRegistration {
public:
  DummyRegistration(const char *name, const char *service, uint port) : ZeroConfRegistration(name, service, port), stream_(std::ostringstream::out) {}

  ~DummyRegistration() {
    stop();
  }

  virtual void registration_done() {
    stream_ << "[registered: " << name_ << " @ " << port_ << "]";
  }

  const std::string str() { return stream_.str(); }
  void str(const char *string) { stream_.str(string); }

private:
  std::ostringstream stream_;
};

class ZeroConfTest : public TestHelper {
 public:

  void test_register_browse( void ) {
    DummyBrowser browser("_oscit._udp");
    browser.adopt_proxy_factory(new ProxyFactory);
    Logger logger;
    CommandLogger cmd("oscit", &logger);
    browser.set_command(&cmd);
    wait(1000);
    browser.lock();
      browser.str(""); // clear
    browser.unlock();
    DummyRegistration *registration = new DummyRegistration("foobar", "_oscit._udp", 5007);

    wait(1500);
    registration->lock();
    browser.lock();
      assert_equal("[registered: foobar @ 5007]", registration->str());
      assert_equal("[+ oscit://\"foobar\"]", browser.str());
      browser.str(""); // clear
    browser.unlock();
    registration->unlock(); // ?
    delete registration;
    wait(2000);
    browser.lock();
      assert_equal("[- oscit://\"foobar\"]", browser.str());
    browser.unlock();
  }

 private:
  void wait(uint milliseconds) {
    int loop_count = milliseconds / 200;
    for(int i=0; i < loop_count; ++i) {
      millisleep(200);
      fprintf(stdout, "-");
      fflush(stdout);
    }
    fprintf(stdout, ".");
    fflush(stdout);
  }
};

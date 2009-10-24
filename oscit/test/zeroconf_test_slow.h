#include <sstream>

#include "test_helper.h"
#include "oscit/zeroconf.h"
#include "ip/UdpSocket.h"

class DummyBrowser : public ZeroConfBrowser {
public:
  DummyBrowser(const char *service) : ZeroConfBrowser(service), stream_(std::ostringstream::out) {}

  ~DummyBrowser() {
    stop();
  }

  virtual void add_device(const Location &location) {
    // only record first entry in case there are more then one network interfaces
    stream_ << "[+ " << location.name() << " @ " << location.port() << "]";
  }

  virtual void remove_device(const char *name, bool more_coming) {
    // only record first entry in case there are more then one network interfaces
    stream_ << "[- " << name << "]";
  }

  const std::string str() { return stream_.str(); }
  void str(const char *string) { stream_.str(string); }

private:
  std::ostringstream stream_;
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
    wait(2000);
    browser.lock();
      browser.str(""); // clear
    browser.unlock();
    DummyRegistration *registration = new DummyRegistration("foobar", "_oscit._udp", 5007);

    wait(2000);
    registration->lock();
    browser.lock();
      assert_equal("[registered: foobar @ 5007]", registration->str());
      assert_equal("[+ foobar @ 5007]", browser.str());
      browser.str(""); // clear
    browser.unlock();
    registration->unlock(); // ?
    delete registration;
    wait(1500);
    browser.lock();
      assert_equal("[- foobar]", browser.str());
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

#include <sstream>

#include "test_helper.h"
#include "oscit/zeroconf.h"
#include "ip/UdpSocket.h"

class DummyBrowser : public ZeroConfBrowser {
public:
  DummyBrowser(const char *service) : ZeroConfBrowser(service), stream_(std::ostringstream::out), was_more_coming_(false) {}
  
  
  virtual void add_device(const char *name, const char *host, unsigned int port, bool more_coming) {
    if (!was_more_coming_) {
      // only record first entry in case there are more then one network interfaces
      stream_ << "[+ " << name << " @ " << host << ":" << port << "]";
    }
    was_more_coming_ = more_coming;
  }
  
  virtual void remove_device(const char *name, const char *host, unsigned int port, bool more_coming) {
    if (!was_more_coming_) {
      // only record first entry in case there are more then one network interfaces
      stream_ << "[- " << name << " @ " << host << ":" << port << "]";
    }
    was_more_coming_ = more_coming;
  }
  
  const std::string str() { return stream_.str(); }
  void str(const char *string) { stream_.str(string); }
  
private:
  std::ostringstream stream_;
  bool was_more_coming_;
};

class DummyRegistration : public ZeroConfRegistration {
public:
  DummyRegistration(const char *name, const char *service, uint port) : ZeroConfRegistration(name, service, port), stream_(std::ostringstream::out) {}
  
  virtual void registration_done() {
    stream_ << "[registered: " << name_ << " @ " << host_ << ":" << port_ << "]";
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
    wait(1000);
    browser.str(""); // clear
    DummyRegistration *registration = new DummyRegistration("foobar", "_oscit._udp", 5007);
    
    wait(2000);
    
    assert_equal("[registered: foobar @ local.:5007]", registration->str());
    assert_equal("[+ foobar @ local.:5007]", browser.str());
    browser.str(""); // clear
    
    delete registration;
    wait(2000);
    assert_equal("[- foobar @ local.:5007]", browser.str());
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

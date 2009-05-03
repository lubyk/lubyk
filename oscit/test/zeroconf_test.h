#include "test_helper.h"
#include "oscit/zeroconf.h"
#include "ip/UdpSocket.h"

class ZeroConfTest : public TestHelper
{
 public:
  
  void test_register_browse( void ) {
    ZeroConfBrowser browser("_oscit._udp");
    ZeroConfRegistration *registration = new ZeroConfRegistration("foobar", "_oscit._udp", 70707);
    //browser should get 'add' notification
    sleep(2);
    delete registration;
    //browser should get 'remove' notification
    sleep(2);
  }
};

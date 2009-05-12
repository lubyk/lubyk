

#include <iostream>
#include <string>
#include "oscit/zeroconf.h"
#include "oscit/thread.h"

using namespace oscit;

class DummyRegistration : public ZeroConfRegistration {
public:
  DummyRegistration() : ZeroConfRegistration(std::string("foobar"), "_oscit._udp", 5009) {}

  void registration_done() {
    printf("Registered '%s'\n", name_.c_str());
  }
};

class Foo : public Thread {
public:
  void loop(Thread *thread) {
    printf("looping..\n");
  }
};

int main(int argc, char** argv)
{
  std::cout << "started\n";
  Foo *reg;
  for (int i=0; i < 300; ++i) {
    reg = new Foo();
    reg->start<Foo, &Foo::loop>(reg, NULL);
    delete reg;
  }
	return 0;
}

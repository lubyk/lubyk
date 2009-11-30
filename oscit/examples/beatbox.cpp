// compile with
// g++ -I../include ../build/liboscit.a beatbox.cpp -o beatbox

#include <stdio.h>
#include <iostream>
#include "oscit/oscit.h"
using namespace oscit;


#define OSC_PORT 7021

class Tempo : public Object
{
public:
  Tempo(const char *name, Real current_value) : Object(name, RangeIO(0, 512, "bpm", "Current tempo.")), tempo_(current_value) {}

  /** This is the method triggered in response to the object's url.
   *  In this example this url is "/tempo".
   */
  virtual const Value trigger(const Value &val, const Location *origin) {
    if (val.is_real()) {
      tempo_.r = val.r;
    }

    std::cout << "tempo: " << tempo_ << std::endl;

    // oscit convention is to return current value
    return tempo_;
  }

private:
  RealValue tempo_;
};

bool gRun = true;

void terminate(int sig) {
  gRun = false;
}

int main(int argc, char * argv[]) {
  Root root("beatbox");

  // open osc command on port OSC_PORT
  root.adopt_command(new OscCommand("oscit", "_oscit._udp", OSC_PORT));

  // create '/tempo' url
  root.adopt(new Tempo("tempo", 120));

  printf("Beatbox started and listening on port %i.\nType Ctrl+C to stop.\n", OSC_PORT);

  // register signals
  signal(SIGINT,  terminate);

  while (gRun) {
    sleep(1);
  }

  printf("Bye...\n");
}
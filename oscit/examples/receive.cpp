// compile with
// g++ -I../include ../build/liboscit.a ../oscpack/liboscpack.a receive.cpp -o receive

#include <stdio.h>
#include <iostream>
#include "oscit/oscit.h"
using namespace oscit;


#define OSC_PORT 7020

class Message : public Object
{
public:
  Message(const char *name, const char *message) : Object(name, StringIO("a message", "Stores a message (string only).")), message_(message) {}
  
  /** This is the method triggered in response to the object's url.
   *  In this example this url is "/message".
   */
  virtual const Value trigger(const Value &val, const Location *origin) {
    std::cout << "[" << url() << "] received " << val << std::endl;
    if (val.is_string()) {
      message_ = val.str();
    }
    
    // oscit convention is to return current value
    return Value(message_);
  }
  
private:
  std::string message_;
};

bool gRun = true;

void terminate(int sig) {
  gRun = false;
}

int main(int argc, char * argv[]) {
  Root root;
  
  // open osc command on port OSC_PORT
  root.adopt_command(new OscCommand(OSC_PORT));
  
  // create '/message' url
  root.adopt(new Message("message", "message in a bottle"));
  
  printf("Simple started and listening on port %i.\nType Ctrl+C to stop.\n", OSC_PORT);
  
  // register signals
  signal(SIGINT,  terminate);
  
  while (gRun) {
    sleep(1);
  }
  
  printf("Bye...\n");
}
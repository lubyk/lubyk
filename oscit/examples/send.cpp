// compile with
// g++ -I../include ../build/liboscit.a send.cpp -o send

/** This simple application sends arbitrary values through osc. */

#include <stdio.h>
#include <stdlib.h>  // atoi
#include <iostream>

#include "oscit/oscit.h"

using namespace oscit;


#define OSC_PORT 7000

bool gRun = true;

void terminate(int sig) {
  gRun = false;
}

void input(const char *name, std::string &rval) {
  std::string tmp;
  printf("%s %18s: ", name, rval.c_str());
  std::getline(std::cin, tmp);
  if (tmp != "") rval = tmp;
}


class ShowReplies : public Object
{
public:
  ShowReplies(const char * name) : Object(name, AnyIO("Log any information.")) {}

  virtual const Value trigger(const Value &val, const Location *origin) {
    std::cout << "[" << url() << "] received " << val << std::endl;
    return gNilValue;
  }
};


int main(int argc, char * argv[]) {
  Root root;
  std::string port_str("7020");
  int port;
  int in_port;

  if (argc > 1) {
    in_port = atoi(argv[1]);
  } else {
    in_port = OSC_PORT;
  }

  std::string url_str("oscit://\"receive\"/message");
  std::string value_str("null");
  std::string continue_input("yes");
  Value value;

  // open osc command on port OSC_PORT
  OscCommand *sender = root.adopt_command(new OscCommand(in_port));

  ZeroConfBrowser browser("_oscit._udp");
  browser.set_command(sender);
  // FIXME: the lines above should be:
  // ZeroConfBrowser *browser = sender->adopt_zeroconf_browser(new ZeroConfBrowser("_oscit._udp"));

  // just to log replies
  root.adopt(new ShowReplies(".reply"));

  // register signals
  signal(SIGINT,  terminate);

  printf("Send started and listening on port %i.\nValues are parsed as Json\n", in_port);
  while (gRun) {
    input("Enter   url", url_str);
    input("Enter value", value_str);
    value.set(Json(value_str));
    Url url(url_str);
    // If the location is a service name, resolve
    url.resolve_with(&browser);

    std::cout << url.location().inspect() << "\n";

    // This is the interesting part: send an osc value out
    sender->send(url, value);

    Thread::millisleep(10); // wait for answer

    input("Continue ? ", continue_input);
    if (continue_input != "yes") {
      gRun = false;
    }
  }

  printf("Bye...\n");
}
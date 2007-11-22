#include "rubyk.h"
#include "command.h"
#include <csignal>
#include <iostream>
#include <fstream>

Rubyk * gServer;

void term(int sig)
{
  gServer->quit();
}

int main(int argc, char * argv[])
{
  gServer = new Rubyk;
  
  if (argc > 1) {
    std::ifstream in(argv[1], std::ios::in);
    std::ostringstream oss;
    oss << in.rdbuf();
    in.close();
    
    Command fCmd(std::cin, std::cout);
    fCmd.set_server(*gServer);
    fCmd.set_silent(true);
    gServer->unlock(); // so the commands are directly processed
      oss << "\n";
      fCmd.parse(oss.str());
    gServer->lock();
    
    fCmd.close();
  }
  
  InteractiveCommand mCmd(std::cin, std::cout);
  gServer->listen_to_command(mCmd);
  signal(SIGTERM, term); // register a SIGTERM handler
  signal(SIGINT,  term);
  while (gServer->run());
  
  delete gServer;
  
  return 0;
}
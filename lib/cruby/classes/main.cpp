#include "rubyk.h"
#include "command.h"
#include <csignal>

Rubyk * gServer;

void term(int sig)
{
  gServer->quit();
}

int main()
{
  gServer = new Rubyk;
  InteractiveCommand mCmd(std::cin, std::cout);
  gServer->listen_to_command(mCmd);
  signal(SIGTERM, term); // register a SIGTERM handler
  signal(SIGINT,  term);
  while (gServer->run());
  delete gServer;
}
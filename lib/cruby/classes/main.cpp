#include "rubyk.h"
#include "command.h"

int main()
{
  Rubyk mServer;
  InteractiveCommand mCmd(std::cin, std::cout);
  mServer.listen_to_command(mCmd);
  while (mServer.run());
}
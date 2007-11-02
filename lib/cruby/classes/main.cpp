#include "rubyk.h"
#include "command.h"

int main()
{
  Rubyk mServer;
  InteractiveCommand mCmd(&mServer);
  mCmd.listen(std::cin, std::cout);
  while (mServer.run());
  mCmd.close(); 
}
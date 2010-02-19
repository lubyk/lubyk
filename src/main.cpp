#include "rubyk.h"
#include "text_command.h"
#include <iostream>

#define VIEW_PATH "."

int main(int argc, char * argv[])
{
  Planet venus(argc, argv);

  // create '/views' url
  Value error;
  if (!venus.expose_views(VIEW_PATH, &error)) {
    std::cout << error << "\n";
    return -1;
  }

  venus.adopt_command(new CommandLine(std::cin, std::cout));
  venus.adopt_command(new OscCommand("oscit", "_oscit._udp"));
  venus.start_worker();
  venus.wait_for_gui();

  return 0;
}
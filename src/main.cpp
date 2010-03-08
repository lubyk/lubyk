#include "rubyk.h"
#include <iostream>

#define VIEW_PATH "."

int main(int argc, char * argv[])
{
  Planet venus;

  // create '/views' url
  Value error;
  if (!venus.expose_views(VIEW_PATH, &error)) {
    std::cout << error << "\n";
    return -1;
  }

  if (argc > 1) {
    std::string file_name(argv[1]);
    venus.set_name(file_name.substr(0, file_name.rfind(".")));

    std::ifstream in(argv[1], std::ios::in);
    std::ostringstream oss;
    oss << in.rdbuf();
    in.close();

    std::cout << oss.str() << "\n";

    venus.adopt_command(new CommandLine(std::cin, std::cout, oss.str()));
  } else {
    venus.adopt_command(new CommandLine(std::cin, std::cout));
  }

  venus.adopt_command(new OscCommand("oscit", "_oscit._udp"));
  venus.start_worker();

  // YOU *MUST* FINISH WITH THIS LINE
  venus.ready();

  return 0;
}
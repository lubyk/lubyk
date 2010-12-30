#include <string>
#include <unistd.h>
#include <iostream>

int main(int argc, char *argv[])
{
  std::string path_to_app(argv[0]);
  size_t contents_pos = path_to_app.rfind("/Contents/MacOS");
  if (contents_pos == std::string::npos) {
    std::cerr << "Could not find path to mimas_app.lua (could not find /Contents/MacOS)" << std::endl;
    return -1;
  }
  std::string path_to_mimas = path_to_app.substr(0, contents_pos);
  path_to_mimas.append("/Contents/Resources/mimas_app.lua");
  return execl("/usr/local/bin/lua", "/usr/local/bin/lua", path_to_mimas.c_str(), (char *) 0);
}

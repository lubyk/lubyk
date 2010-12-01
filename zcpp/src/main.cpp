/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
   Copyright (c) 2007-2010 by Gaspard Bucher - Buma (http://teti.ch).

  ------------------------------------------------------------------------------

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

  ==============================================================================
*/

#include "rubyk.h"
#include <iostream>

using namespace rk;

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
/*
  ==============================================================================

   This file is part of the OSCIT library (http://rubyk.org/liboscit)
   Copyright (c) 2007-2009 by Gaspard Bucher - Buma (http://teti.ch).

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

#include "oscit/file.h"

#include <string>
#include <sstream>
#include <fstream>    // file io


namespace oscit {

bool File::write(const std::string &content) {
  // FIXME: atomic !!
  std::ofstream out(path_.c_str(), std::ios::out);
    if (out.fail()) {
      last_error_ = ErrorValue(UNKNOWN_ERROR, std::string("Could not write to file '").append(path_).append("'."));
      return false;
    }
    out << content;
  out.close();
  return true;
}

Value File::read() {
  std::ostringstream oss;
  // FIXME: atomic !!
  std::ifstream in(path_.c_str(), std::ios::in);
    if (in.fail()) {
      last_error_ = ErrorValue(UNKNOWN_ERROR, std::string("Could not read file '").append(path_).append("'."));
      return last_error_;
    }
    oss << in.rdbuf();
  in.close();
  return Value(oss.str());
}

bool File::append(const std::string& data) {
  // FIXME: atomic !!
  std::ofstream out(path_.c_str(), std::ios_base::app);
    if (out.fail()) {
      last_error_ = ErrorValue(UNKNOWN_ERROR, std::string("Could not append to file '").append(path_).append("'."));
      return false;
    }
    out << data;
  out.close();
  return true;
}

} // oscit

/*
  ==============================================================================

   This file is part of the RUBYK library (http://rubyk.org)
   Copyright (c) 2007-2011 by Gaspard Bucher (http://teti.ch).

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

#ifndef RUBYK_INCLUDE_RUBYK_EXCEPTION_H_
#define RUBYK_INCLUDE_RUBYK_EXCEPTION_H_

#include <exception>

#define EXCEPTION_BUFFER_SIZE 256

namespace rubyk {

class Exception : public std::exception
{
  std::string message_;
public:
  explicit Exception(const char *format, ...) {
    char buffer[EXCEPTION_BUFFER_SIZE];
    va_list args;
    va_start(args, format);
      vsnprintf(buffer, EXCEPTION_BUFFER_SIZE, format, args);
    va_end(args);
    message_ = buffer;
  }

  ~Exception() throw() {}

  const char* what() const throw() {
    return message_.c_str();
  }
};

} // rubyk

#endif // RUBYK_INCLUDE_RUBYK_EXCEPTION_H_
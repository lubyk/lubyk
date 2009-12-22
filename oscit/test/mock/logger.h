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

#ifndef OSCIT_TEST_MOCK_LOGGER_H_
#define OSCIT_TEST_MOCK_LOGGER_H_

#include <sstream>

class Logger : public std::ostringstream {
public:
  Logger() : std::ostringstream(std::ostringstream::out) {}
};

class MockLogger {
public:
  MockLogger(const char *identifier, std::ostream *stream) : identifier_(identifier), stream_(stream) {}

protected:
  template<class T>
  void log(const T& info) {
    if (!stream_) return;
    *stream_ << "[" << identifier_ << ": " << info << "]";
  }

  template<class S, class T>
  void log(const S& info1, const T& info2) {
    if (!stream_) return;
    *stream_ << "[" << identifier_ << ": " << info1 << " " << info2 << "]";
  }

  template<class S, class T, class U>
  void log(const S& info1, const T& info2, const U& info3) {
    if (!stream_) return;
    *stream_ << "[" << identifier_ << ": " \
             << info1 << " " \
             << info2 << " " \
             << info3 << "]";
  }

  template<class S, class T, class U, class V>
  void log(const S& info1, const T& info2, const U& info3, const V& info4) {
    if (!stream_) return;
    *stream_ << "[" << identifier_ << ": " \
             << info1 << " " \
             << info2 << " " \
             << info3 << " " \
             << info4 << "]";
  }

  void set_stream(std::ostream *stream) {
    stream_ = stream;
  }

  std::string identifier_;

private:
  std::ostream *stream_;
};


#endif // OSCIT_TEST_MOCK_LOGGER_H_

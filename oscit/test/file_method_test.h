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

#include "test_helper.h"
#include "oscit/root.h"
#include "oscit/file_method.h"

#include <sstream>
#include <fstream>    // file io

#define FILE_METHOD_PATH "simple_view.json"

class FileMethodTest : public TestHelper
{
public:
  FileMethodTest() {
    preserve(fixture_path(FILE_METHOD_PATH));
  }

  void tearDown() {
    restore(fixture_path(FILE_METHOD_PATH));
  }

  void test_read( void ) {
    Root root(false);
    root.adopt(new FileMethod("simple_view", fixture_path(FILE_METHOD_PATH), std::string("Basic synth view.")));
    Value res = root.call("/simple_view");
    assert_true(res.is_string())
    assert_equal("{\n  \"x\":0, \"y\":0, \"width\":500, \"height\":", res.str().substr(0, 40));
  }

  void test_write( void ) {
    Root root(false);
    root.adopt(new FileMethod("simple_view", fixture_path(FILE_METHOD_PATH), std::string("Basic synth view.")));
    Value res = root.call("/simple_view", Value("Yoba"));
    assert_true(res.is_string());
    assert_equal("Yoba", res.str());

    res = root.call("/simple_view");
    assert_true(res.is_string());
    assert_equal("Yoba", res.str());

    std::ifstream in(fixture_path(FILE_METHOD_PATH).c_str(), std::ios::in);
      std::ostringstream oss;
      oss << in.rdbuf();
    in.close();
    assert_equal("Yoba", oss.str());
  }
};


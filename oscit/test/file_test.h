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
#include "oscit/file.h"

#define FILE_TEST_PATH "simple_view.json"
class FileTest : public TestHelper
{
public:
  FileTest() {
    preserve(fixture_path(FILE_TEST_PATH));
  }

  void tearDown() {
    restore(fixture_path(FILE_TEST_PATH));
  }

  void test_read( void ) {
    File file(fixture_path(FILE_TEST_PATH));
    Value content = file.read();
    assert_true(content.is_string())
    assert_equal("{\n  \"x\":0, \"y\":0, \"width\":500, \"height\":", content.str().substr(0, 40));
  }

  void test_write( void ) {
    File file1(fixture_path(FILE_TEST_PATH));
    File file2(fixture_path(FILE_TEST_PATH));
    assert_true(file1.write("Yoba"));
    Value content = file2.read();
    assert_true(content.is_string())
    assert_equal("Yoba", content.str());
  }

  void test_append( void ) {
    File file(fixture_path(FILE_TEST_PATH));
    file.write("");
    file.append("Hello");
    file.append(" World!");
    assert_equal("Hello World!", file.read().str());
  }

  void test_do_not_keep_handle( void ) {
    File file(fixture_path(FILE_TEST_PATH));
    File file2(fixture_path(FILE_TEST_PATH));
    std::string new_content = "dummy new content";
    file2.write(new_content);
    assert_equal(new_content, file.read().str());
  }
};


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
#include "oscit/hash_file_method.h"

#include <sstream>
#include <fstream>    // file io

#define HASH_FILE_METHOD_PATH "simple_view.json"

class HashFileMethodTest : public TestHelper
{
public:
  HashFileMethodTest() {
    preserve(fixture_path(HASH_FILE_METHOD_PATH));
  }

  void tearDown() {
    restore(fixture_path(HASH_FILE_METHOD_PATH));
  }

  void test_read( void ) {
    Root root(false);
    root.adopt(new HashFileMethod("simple_view", fixture_path(HASH_FILE_METHOD_PATH), std::string("Basic synth view.")));
    Value res = root.call("/simple_view");
    assert_true(res.is_hash())
    assert_equal("{\"x\":0, \"y\":0, \"width\":500, \"height\":200", res.to_json().substr(0, 40));
  }

  void test_write_hash( void ) {
    Root root(false);
    root.adopt(new HashFileMethod("simple_view", fixture_path(HASH_FILE_METHOD_PATH), std::string("Basic synth view.")));
    Value res = root.call("/simple_view", Value(Json("{\"one\":1}")));
    assert_true(res.is_hash());
    assert_equal("{\"one\":1}", res.to_json());

    res = root.call("/simple_view");
    assert_true(res.is_hash());
    assert_equal("{\"one\":1}", res.to_json());

    std::ifstream in(fixture_path(HASH_FILE_METHOD_PATH).c_str(), std::ios::in);
      std::ostringstream oss;
      oss << in.rdbuf();
    in.close();
    assert_equal("{\"one\":1}", oss.str());
  }

  void test_write_bad_format( void ) {
    Root root(false);
    HashFileMethod *fm = root.adopt(new HashFileMethod("simple_view", fixture_path(HASH_FILE_METHOD_PATH), std::string("Basic synth view.")));
    assert_false(fm->can_receive(Value(Json("Yoba"))));
    Value res = root.call("/simple_view", Value("Yoba"));
    assert_true(res.is_error());
    assert_equal(BAD_REQUEST_ERROR, res.error_code());

    // content not changed
    res = root.call("/simple_view");
    assert_true(res.is_hash());
    assert_equal("{\"x\":0, \"y\":0, \"width\":500, \"height\":200", res.to_json().substr(0, 40));

    std::ifstream in(fixture_path(HASH_FILE_METHOD_PATH).c_str(), std::ios::in);
      std::ostringstream oss;
      oss << in.rdbuf();
    in.close();
    assert_equal("{\n  \"x\":0, \"y\":0, \"width\":500, \"height\":", oss.str().substr(0, 40));
  }

  void test_update( void ) {
    Root root(false);
    root.adopt(new HashFileMethod("simple_view", fixture_path(HASH_FILE_METHOD_PATH), std::string("Basic synth view.")));
    Value res = root.call("/simple_view/update", Value(Json("{parts:{1:{x:11}}}")));
    assert_true(res.is_hash());
    assert_equal("{\"parts\":{\"1\":{\"x\":11}}}", res.to_json());

    res = root.call("/simple_view");
    assert_true(res.is_hash());
    assert_equal("\"parts\":{\"1\":{\"class\":\"Slider\", \"x\":11, \"y\":10, \"width\":30", res.to_json().substr(42, 58));
    assert_equal(11, res["parts"]["1"]["x"].r);

    std::ifstream in(fixture_path(HASH_FILE_METHOD_PATH).c_str(), std::ios::in);
      std::ostringstream oss;
      oss << in.rdbuf();
    in.close();
    assert_equal("\"parts\":{\"1\":{\"class\":\"Slider\", \"x\":11, \"y\":10, \"width\":30", oss.str().substr(42, 58));
  }
};


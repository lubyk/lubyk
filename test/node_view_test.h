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

#include "test_helper.h"

class NodeViewTest : public TestHelper {
public:
  void test_from_hash( void ) {
    NodeView view;
    HashValue res;
    view.from_hash(Value(Json("{x:45, y:12, width:200, height:25, foo:444, hue:13}")), &res);
    assert_equal("{\"x\":45, \"y\":12, \"width\":200, \"height\":25, \"hue\":13}", res.to_json());
    assert_equal(45.0, view.x_);
    assert_equal(12.0, view.y_);
    assert_equal(200.0, view.width_);
    assert_equal(25.0, view.height_);
    assert_equal(13.0, view.hue_);
  }

  void test_to_hash( void ) {
    assert_equal("{\"x\":10.5, \"y\":3.2, \"width\":90, \"height\":120}", NodeView(10.5, 3.2, 90, 120, 85).to_hash().to_json());
  }

  void test_insert_in_hash_result( void ) {
    Value hash;
    NodeView view(10.5, 3.2, 90, 120, 85);
    view.insert_in_hash(&hash);

    assert_equal("{\"x\":10.5, \"y\":3.2, \"width\":90, \"height\":120, \"hue\":85}", hash.to_json());
  }

  void test_to_stream( void ) {
    NodeView view(10.5, 3.2, 90, 120, 85);
    std::ostringstream out(std::ostringstream::out);
    out << view;
    assert_equal("{\"x\":10.5, \"y\":3.2, \"width\":90, \"height\":120, \"hue\":85}", out.str());
  }
};

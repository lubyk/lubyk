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

class PlanetTest : public ParseHelper {
public:
  void test_to_hash( void ) {
    setup("m = Metro(115)\nv = Value(15)\nm => v\n");
    Value patch_def = planet_->to_hash();
    assert_equal("{\"x\":10, \"y\":10, \"width\":500, \"height\":300}", patch_def[Attribute::VIEW].to_json());
    // just to make sure objects are inserted
    assert_equal("115", patch_def[NODES_KEY]["m"]["tempo"].to_json());
  }

  // create a node
  void test_set( void ) {
    Value res = planet_->set(JsonValue("{nodes:{foo:{\"@class\":\"/class/Value\"}}}"));
    ObjectHandle foo;
    assert_true(planet_->get_object_at("/foo", &foo));
    Value foo_hash = foo->to_hash();
    assert_equal("/class/Value", foo_hash["@class"].str());
  }

  // create a link
  void test_create_link( void ) {
    setup_with_print("p = Print()\nv = Value(15)\n"); // n => p by default so "v" is not linked to p
    Value res = planet_->set(Value(Json("{nodes:{v:{out:{value:{\"/p/print\":{}}}}}}")));
    assert_equal("{}", res["nodes"]["v"]["out"]["value"]["/p/print"].to_json());

    assert_print("p: 123\n", "v(123)\n");
  }
};
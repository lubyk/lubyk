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
#include "mock/dummy_object.h"

class TreeMetaMethodTest : public TestHelper
{
public:
  /* Bernoulli family
  /Nikolaus
  /Nikolaus/Jacob
  /Nikolaus/Nikolaus
  /Nikolaus/Johann
  /Nikolaus/Johann/Nicolaus
  /Nikolaus/Johann/Daniel
  /Nikolaus/Johann/Johann
   */
  void test_tree( void ) {
    Root root;
    Object * tmp = root.adopt(new Object("Nikolaus"));
    tmp->adopt(new Object("Jacob"));
    tmp->adopt(new Object("Nikolaus"));
    tmp = tmp->adopt(new Object("Johann"));
    tmp->adopt(new Object("Nicolaus"));
    tmp->adopt(new Object("Daniel"));
    tmp->adopt(new Object("Johann"));
    Value res;

    res = root.call(TREE_PATH, Value(""));
    assert_equal("", res[0].str());
    res = res[1];
    assert_equal(13, res.size());
    assert_equal("[\".error\", \".info\", \".list\", \".list_with_type\", \".type\", \".tree\", \"Nikolaus\", \"Nikolaus/Jacob\", \"Nikolaus/Nikolaus\", \"Nikolaus/Johann\", \"Nikolaus/Johann/Nicolaus\", \"Nikolaus/Johann/Daniel\", \"Nikolaus/Johann/Johann\"]", res.to_json());
    
    res = root.call(TREE_PATH, Value("/Nikolaus"));
    assert_equal("/Nikolaus", res[0].str());
    res = res[1];
    assert_equal(6, res.size());
    assert_equal("[\"Jacob\", \"Nikolaus\", \"Johann\", \"Johann/Nicolaus\", \"Johann/Daniel\", \"Johann/Johann\"]", res.to_json());
    
    res = root.call(TREE_PATH, Value("/Nikolaus/Johann"));
    assert_equal("/Nikolaus/Johann", res[0].str());
    res = res[1];
    assert_equal(3, res.size());
    assert_equal("[\"Nicolaus\", \"Daniel\", \"Johann\"]", res.to_json());
    Value res2 = root.call(TREE_PATH, Value("/Nikolaus/Johann"));
    res2 = res2[1];
    assert_equal(res2.to_json(), res.to_json()); // should be the same as list if there are no children
  }
  
  void test_tree_on_empty( void ) {
    Root root;
    root.adopt(new Object("Zorglub"));
    Value res;

    res = root.call(TREE_PATH, Value("/Zorglub"));
    assert_equal("/Zorglub", res[0].str());
    res = res[1];
    assert_equal(0, res.size());
  }
  
  void test_tree_with_nil( void ) {
    Root root(NoIO("This is the root node."));
    Value res;
    
    res = root.call(TREE_PATH, gNilValue);
    assert_true(res.is_nil());
  }
};
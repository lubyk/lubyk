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

class ListMetaMethodTest : public TestHelper
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
  void test_list( void ) {
    Root root;
    Object * tmp = root.adopt(new Object("Nikolaus"));
    tmp->adopt(new Object("Jacob"));
    tmp->adopt(new Object("Nikolaus"));
    tmp = tmp->adopt(new Object("Johann"));
    tmp->adopt(new Object("Nicolaus"));
    tmp->adopt(new Object("Daniel"));
    tmp->adopt(new Object("Johann"));
    Value reply;

    reply = root.call(LIST_PATH, Value(""));
    assert_equal("", reply[0].str());
    Value res = reply[1];
    
    assert_true(res.is_list());
    assert_equal(7, res.size());
    assert_equal(Url(ERROR_PATH).name(), res[0].str());
    assert_equal(Url(INFO_PATH).name(), res[1].str());
    assert_equal(Url(LIST_PATH).name(), res[2].str());
    assert_equal(Url(LIST_WITH_TYPE_PATH).name(), res[3].str());
    assert_equal(Url(TYPE_PATH).name(), res[4].str());
    assert_equal(Url(TREE_PATH).name(), res[5].str());
    assert_equal("Nikolaus/", res[6].str());
    
    reply = root.call(LIST_PATH, Value("/Nikolaus"));
    assert_equal("/Nikolaus", reply[0].str());
    res = reply[1];
    assert_true(res.is_list());
    assert_equal(3, res.size());
    assert_equal("Jacob",    res[0].str());
    assert_equal("Nikolaus", res[1].str());
    assert_equal("Johann/",  res[2].str());
    
    reply = root.call(LIST_PATH, Value("/Nikolaus/Johann"));
    assert_equal("/Nikolaus/Johann", reply[0].str());
    res = reply[1];
    assert_true(res.is_list());
    assert_equal(3, res.size());
    assert_equal("Nicolaus", res[0].str());
    assert_equal("Daniel",   res[1].str());
    assert_equal("Johann",   res[2].str());
  }
  
  void test_list_on_empty( void ) {
    Root root;
    root.adopt(new Object("Nikolaus"));
    Value res;

    res = root.call(LIST_PATH, Value("/Nikolaus"));
    assert_equal("/Nikolaus", res[0].str());
    res = res[1];
    assert_true(res.is_list());
    assert_equal(0,  res.size());
  }
  
  void test_list_with_nil( void ) {
    Root root(NoIO("This is the root node."));
    Value res;
    
    res = root.call(LIST_PATH, gNilValue);
    assert_true(res.is_nil());
  }
};
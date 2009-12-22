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
#include "oscit/tsmart_ptr.h"

/** Simple class to test the smart pointer template. */
class TestSmartData
{
public:
  TestSmartData(int value, const char ** statusNotification = NULL) : value_(value), statusNotification_(statusNotification)
  {
    if (statusNotification_ != NULL)
      *statusNotification_ = "created";
  }
  
  ~TestSmartData()
  {
    if (statusNotification_ != NULL)
      *statusNotification_ = "deleted";
  }
  
  int value_;
  const char ** statusNotification_;
};

class TestSmart : public TSmartPtr<TestSmartData>
{
public:
  TestSmart(int value) : TSmartPtr<TestSmartData>(new TestSmartData(value)) {}
  TestSmart(int value, const char ** notification) : TSmartPtr<TestSmartData>(new TestSmartData(value, notification)) {}
};

class SmartPointerTest : public TestHelper
{  
public:
  void test_create( void )
  { 
    TestSmart s(5);
    assert_equal(5, s.data()->value_);
  }
  
  void testRefCount( void )
  {
    TestSmart s(5);
    assert_equal(1, s.ref_count());
    assert_equal(5, s.data()->value_);
    
    TestSmart * s2 =  new TestSmart(s);
    assert_equal(2, s.ref_count());
    assert_equal(2, s2->ref_count());
    assert_equal(5, s2->data()->value_);
    
    s2->data()->value_ = 3;
    assert_equal(3, s2->data()->value_);
    assert_equal(3, s.data()->value_);
    
    delete s2;
    assert_equal(1, s.ref_count());
  }
  
  void testDelete( void )
  {
    const char * notification;
    TestSmart * s = new TestSmart(3, &notification);
    assert_equal("created", notification);
    notification = "";
    // should not create a new TestSmartData
    TestSmart * s2 = new TestSmart(*s);
    assert_equal("", notification);
    delete s;
    // should not delete the TestSmartData
    assert_equal("", notification);
    delete s2;
    assert_equal("deleted", notification);
  }
};
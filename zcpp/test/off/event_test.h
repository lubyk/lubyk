/*
  ==============================================================================

   This file is part of the LUBYK project (http://lubyk.org)
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

// ordered_list_test.h 
#include <cxxtest/TestSuite.h>
#include "outlet.h"
#include "inlet.h"
#include "event.h"

struct EventUser
{
  EventUser(int pValue) : value_(pValue) {}
  void increment(void)
  { value_++; }
  void add(void * pValue)
  { value_ = value_ + *((int*)pValue); }
  int value_;
};

// FIXME: complete tests !
class EventTest : public CxxTest::TestSuite
{
public:
  void test_create( void )
  {
    EventUser u(1);
    int val = 5;
    TCallEvent<EventUser, &EventUser::increment> e1(345, &u);
    TEvent<EventUser, &EventUser::add> e2(333, &u, (void *)&val);
    
    
    TS_ASSERT_EQUALS( u.value_, 1);
    e1.trigger();
    TS_ASSERT_EQUALS( u.value_, 2);
    e2.trigger();
    TS_ASSERT_EQUALS( u.value_, 7);
  }
};
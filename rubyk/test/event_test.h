// ordered_list_test.h 
#include <cxxtest/TestSuite.h>
#include "outlet.h"
#include "inlet.h"
#include "event.h"

struct EventUser
{
  EventUser(int pValue) : mValue(pValue) {}
  void increment(void)
  { mValue++; }
  void add(void * pValue)
  { mValue = mValue + *((int*)pValue); }
  int mValue;
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
    
    
    TS_ASSERT_EQUALS( u.mValue, 1);
    e1.trigger();
    TS_ASSERT_EQUALS( u.mValue, 2);
    e2.trigger();
    TS_ASSERT_EQUALS( u.mValue, 7);
  }
};
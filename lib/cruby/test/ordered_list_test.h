// ordered_list_test.h 
#include <cxxtest/TestSuite.h>
#include "orderedList.h"


class TestOrderedList : public CxxTest::TestSuite
{
public:
  void testPushPop( void ) {
    OrderedList<int> list;
    int i1 = 1;
    int i2 = 2;
    int i3 = 3;
    
    list.push(&i2);
    TS_ASSERT_EQUALS( &i2, list.top());
    list.pop();
    TS_ASSERT_EQUALS( (void*)NULL, list.top());
    
    // adding keeps element sorted
    list.push(&i3);
    list.push(&i2);
    TS_ASSERT_EQUALS( &i2, list.top());
    list.push(&i1);
    TS_ASSERT_EQUALS( &i1, list.top());
    list.push(&i1);
    
    TS_ASSERT_EQUALS( &i1, list.top());
    
    // pop to make sure we have a single copy, ordered
    list.pop();
    TS_ASSERT_EQUALS( &i2, list.top());
    list.pop();
    TS_ASSERT_EQUALS( &i3, list.top());
    list.pop();
    TS_ASSERT_EQUALS( (void*)NULL, list.top());
  }
};
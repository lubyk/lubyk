// ordered_list_test.h 
#include <cxxtest/TestSuite.h>
#include "ordered_list.h"


class TestOrderedList : public CxxTest::TestSuite
{
public:
  void testPushPop( void )
  {
    OrderedList<int> list;
    int i;
    
    list.push(2);
    TS_ASSERT_EQUALS( 2, list.front());
    TS_ASSERT( list.get(&i) );
    TS_ASSERT_EQUALS( 2, i);
    list.pop();
    TS_ASSERT( list.empty() );
    TS_ASSERT( !list.get(&i) );
    
    // adding keeps element sorted
    list.push(3);
    list.push(2);
    TS_ASSERT_EQUALS( 2, list.front());
    list.push(1);
    TS_ASSERT_EQUALS( 1, list.front());
    list.push(1);
    
    TS_ASSERT_EQUALS( 1, list.front());
    
    // pop to make sure we have a single copy, ordered
    list.pop();
    TS_ASSERT_EQUALS( 2, list.front());
    list.pop();
    TS_ASSERT_EQUALS( 3, list.front());
    list.pop();
    TS_ASSERT( list.empty());
  }
};
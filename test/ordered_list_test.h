// ordered_list_test.h 
#include <cxxtest/TestSuite.h>
#include "ordered_list.h"


class TestOrderedList : public CxxTest::TestSuite
{
public:
  void test_push_pop( void )
  {
    OrderedList<int> list;
    int i;
    
    TS_ASSERT_EQUALS( list.size(), 0);
    
    list.push(2);
    TS_ASSERT_EQUALS( list.size(), 1);
    TS_ASSERT_EQUALS( list.front(), 2);
    TS_ASSERT( list.get(&i) );
    TS_ASSERT_EQUALS( i, 2);
    list.pop();
    TS_ASSERT( list.empty() );
    TS_ASSERT( !list.get(&i) );
    
    // adding keeps element sorted
    list.push(3);
    list.push(2);
    TS_ASSERT_EQUALS( list.front(), 2);
    list.push(1);
    TS_ASSERT_EQUALS( list.front(), 1);
    list.push(1);
    
    TS_ASSERT_EQUALS( list.front(), 1);
    
    // pop to make sure we have a single copy, ordered
    list.pop();
    TS_ASSERT_EQUALS( list.front(), 2);
    list.pop();
    TS_ASSERT_EQUALS( list.front(), 3);
    list.pop();
    TS_ASSERT( list.empty());
  }
};
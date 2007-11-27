// ordered_list_test.h 
#include <cxxtest/TestSuite.h>
#include "buf.h"


class BufTest : public CxxTest::TestSuite
{
public:
  void tAppend( void )
  {
    Buf<int> buf;
    int i;
    
    TS_ASSERT_EQUALS( buf.size, 0);
    
    buf.append(2);
    TS_ASSERT_EQUALS( buf.size, 1);
    TS_ASSERT_EQUALS( buf.data[0], 2);
    
    buf.append(5);
    TS_ASSERT_EQUALS( buf.size, 2);
    TS_ASSERT_EQUALS( buf.data[0], 2);
    TS_ASSERT_EQUALS( buf.data[1], 5);
  }
  
  void testAppendDouble( void )
  {
    Buf<double> buf;
    double val;
    val = 1.0;
    TS_ASSERT_EQUALS( buf.size, 0);
    
    TS_ASSERT(buf.append(val));
    TS_ASSERT_EQUALS( buf.size, 1);
    TS_ASSERT_EQUALS( buf.data[0], 1.0);
    
    val = 2.0;
    TS_ASSERT(buf.append(val));
    TS_ASSERT_EQUALS( buf.size, 2);
    TS_ASSERT_EQUALS( buf.data[0], 1.0);
    TS_ASSERT_EQUALS( buf.data[1], 2.0);
  }
};
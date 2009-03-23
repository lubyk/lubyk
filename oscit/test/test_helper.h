#ifndef _TEST_HELPER_H_
#define _TEST_HELPER_H_
#include <cxxtest/TestSuite.h>
#include "oscit/values.h"
#include "oscit/object.h"
#include "oscit/globals.cpp"
//#include "oscit/root.h"

#define assertEqual(x,y) _assertEqual(__FILE__,__LINE__,x,y)
#define assertTrue(e) _TS_ASSERT(__FILE__,__LINE__,e)
#define assertFalse(e) _TS_ASSERT(__FILE__,__LINE__,!(e))

#define _OSCIT_ASSERT_EQUALS(f,l,cmd,x,y) { _TS_TRY { CxxTest::doAssertEquals( (f), (l), (cmd), (x), #y, (y), (0) ); } __TS_CATCH(f,l) }

class TestHelper : public CxxTest::TestSuite
{
protected:
  
  void _assertEqual(const char * file, int lineno, real_t expected, real_t found)
  {
    _OSCIT_ASSERT_EQUALS( file, lineno, TS_AS_STRING(expected), found, expected);
  }
  
  void _assertEqual(const char * file, int lineno, int expected, int found)
  {
    _OSCIT_ASSERT_EQUALS( file, lineno, TS_AS_STRING(expected), found, expected);
  }
  
  void _assertEqual(const char * file, int lineno, const char * expected, const char * found)
  {
    _OSCIT_ASSERT_EQUALS( file, lineno, TS_AS_STRING(expected), found, expected);
  }
  
  void _assertEqual(const char * file, int lineno, const char * expected, const std::string& found)
  {
    _OSCIT_ASSERT_EQUALS( file, lineno, TS_AS_STRING(expected), found, std::string(expected));
  }
  
  void _assertEqual(const char * file, int lineno, void * expected, void * found)
  {
    _OSCIT_ASSERT_EQUALS( file, lineno, TS_AS_STRING(expected), found, expected);
  }
};

#endif // _TEST_HELPER_H_
#ifndef _TEST_HELPER_H_
#define _TEST_HELPER_H_
#include <cxxtest/TestSuite.h>
#include "oscit/oscit.h"
#include "oscit/thread.h"
#include <ostream>

using namespace oscit;

#define TEST_FIXTURES_PATH "../test/fixtures"

std::string fixture_path(const char *path) {
  return std::string(TEST_FIXTURES_PATH).append("/").append(path);
}

void millisleep(float microseconds) {
  Thread::millisleep(microseconds);
}

#define assert_equal(x,y) _assert_equal(__FILE__,__LINE__,#y,x,y)
#define assert_true(e) _TS_ASSERT(__FILE__,__LINE__,e)
#define assert_false(e) _TS_ASSERT(__FILE__,__LINE__,!(e))

#define _OSCIT_ASSERT_EQUALS(f,l,cmd,y,x) { _TS_TRY { CxxTest::doAssertEquals( (f), (l), (cmd), (x), #y, (y), (0) ); } __TS_CATCH(f,l) }

class TestHelper : public CxxTest::TestSuite
{
protected:

  void _assert_equal(const char * file, int lineno, const char * descr, Real expected, Real found)
  {
    _OSCIT_ASSERT_EQUALS( file, lineno, TS_AS_STRING(descr), expected, found);
  }

  void _assert_equal(const char * file, int lineno, const char * descr, int expected, int found)
  {
    _OSCIT_ASSERT_EQUALS( file, lineno, TS_AS_STRING(descr), expected, found);
  }

  void _assert_equal(const char * file, int lineno, const char * descr, uint expected, uint found)
  {
    _OSCIT_ASSERT_EQUALS( file, lineno, TS_AS_STRING(descr), expected, found);
  }

  void _assert_equal(const char * file, int lineno, const char * descr, int expected, uint found)
  {
    _OSCIT_ASSERT_EQUALS( file, lineno, TS_AS_STRING(descr), (uint)expected, found);
  }

  void _assert_equal(const char * file, int lineno, const char * descr, int expected, size_t found)
  {
    _OSCIT_ASSERT_EQUALS( file, lineno, TS_AS_STRING(descr), (size_t)expected, found);
  }

  void _assert_equal(const char * file, int lineno, const char * descr, int expected, time_t found)
  {
    _OSCIT_ASSERT_EQUALS( file, lineno, TS_AS_STRING(descr), (time_t)expected, found);
  }

  void _assert_equal(const char * file, int lineno, const char * descr, unsigned long expected, unsigned long found)
  {
    _OSCIT_ASSERT_EQUALS( file, lineno, TS_AS_STRING(descr), expected, found);
  }

  void _assert_equal(const char * file, int lineno, const char * descr, const char * expected, const char * found)
  {
    _OSCIT_ASSERT_EQUALS( file, lineno, TS_AS_STRING(descr), std::string(expected), std::string(found));
  }

  void _assert_equal(const char * file, int lineno, const char * descr, const char * expected, const std::string &found)
  {
    _OSCIT_ASSERT_EQUALS( file, lineno, TS_AS_STRING(descr), std::string(expected), found);
  }

  void _assert_equal(const char * file, int lineno, const char * descr, const std::string &expected, const std::string &found)
  {
    _OSCIT_ASSERT_EQUALS( file, lineno, TS_AS_STRING(descr), expected, found);
  }

  void _assert_equal(const char * file, int lineno, const char * descr, void * expected, void * found)
  {
    _OSCIT_ASSERT_EQUALS( file, lineno, TS_AS_STRING(descr), expected, found);
  }
};

#endif // _TEST_HELPER_H_
#ifndef _VALUE_TEST_HELPER_H_
#define _VALUE_TEST_HELPER_H_
#include "test_helper.h"
#include "value.h"

#define assert_log(x) _assert_log(__FILE__,__LINE__,x)
#define assert_ref_count(x,y) _assert_ref_count(__FILE__,__LINE__,x,y)
#define assert_id(x,y) _assert_id(__FILE__,__LINE__,x,y)
#define assert_matrix_equal(x,y) _assert_matrix_equal(__FILE__,__LINE__,x,y)

class TestData : public Data
{
public:
  TestData()
  {
    log("new");
  }
  
  virtual ~TestData()
  {
    log("dead");
  }
  
  // copy constructor
  TestData(const TestData& v)
  {
    log("copy");
  }
  
  virtual Data * clone()
  {
    log("clone");
    return new TestData(*this);
  }
  
  virtual value_t type() const
  { return NumberValue; }
  
  static std::ostringstream sOut;
  
protected:
  void log(const char * msg)
  {
    if (sOut.str().size() > 0) sOut << ", ";
    sOut << "[" << mId << "] " << msg;
  }
  
};

class TestValue : public Value
{
public:
  VALUE_METHODS(TestValue, TestData, NumberValue)
};

std::ostringstream TestData::sOut(std::ostringstream::out);
size_t Data::sIdCounter = 0;

class ValueTestHelper : public CxxTest::TestSuite
{
public:
  
  void setUp()
  {
    TestData::sIdCounter = 0;
    TestData::sOut.str(std::string(""));
  }
protected:
  void _assert_log(const char * file, int lineno, const char * pLog)
  {
    _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(std::string("log")), std::string(pLog), TestData::sOut.str());
    TestData::sOut.str(std::string(""));
  }
  
  void _assert_ref_count(const char * file, int lineno, Value& v, size_t count)
  {
    _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(std::string("ref count")), v.ref_count(), count);
  }
  
  void _assert_id(const char * file, int lineno, Value& v, size_t id)
  {
    _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(std::string("id")), v.data_id(), id);
  }
};
#endif // _VALUE_TEST_HELPER_H_
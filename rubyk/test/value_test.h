// loop buffer test
// ordered_list_test.h 
#include <cxxtest/TestSuite.h>
#include "values.h"
#include <sstream>

class TestData : public Data
{
public:
  TestData()
  {
    mId = ++sIdCounter;
    log("create");
  }
  
  virtual ~TestData()
  {
    log("destroy");
  }
  
  // copy constructor
  TestData(const Data& v)
  {
    mId    = ++sIdCounter;
    log("copy");
  }
  
  virtual Data * clone()
  {
    log("clone");
    return new Data(*this);
  }
  
  static std::ostringstream sOut;
  static size_t sIdCounter;
  size_t        mId;
  
protected:
  void log(const char * msg)
  {
    sOut << "[" << mId << "] " << msg << "\n";
  }
  
};

class TestValue : public Value
{
public:
  TestValue(TestData* p) : Value(p) {}
  TestValue() {}
  
  size_t data_id()
  {
    if (mPtr) {
      return ((TestData*)mPtr->mDataPtr)->mId;
    } else {
      return 0;
    }
  }
};

std::ostringstream TestData::sOut(std::ostringstream::out);
size_t TestData::sIdCounter = 0;

class ValueTest : public CxxTest::TestSuite
{
  void setUp()
  {
    TestData::sIdCounter = 0;
    TestData::sOut.str(std::string(""));
  }
  
public:
  void test_create_destroy( void )
  {
    TestValue v1(new TestData);
    assert_log("[1] create\n");
    TestValue v2;
    v1 = v2;
    assert_log("[1] create\n[1] destroy\n");
  }
private:
  void assert_log(const char * pLog)
  {
    TS_ASSERT_EQUALS(std::string(pLog), TestData::sOut.str());
  }
  
  void assert_ref_count(size_t count, Value& v)
  {
    TS_ASSERT_EQUALS(count, v.ref_count());
  }
  
  void assert_id(size_t id, TestValue& v)
  {
    TS_ASSERT_EQUALS(id, v.data_id());
  }
};
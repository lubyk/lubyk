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
    log("new");
  }
  
  virtual ~TestData()
  {
    log("dead");
  }
  
  // copy constructor
  TestData(const TestData& v)
  {
    mId    = ++sIdCounter;
    log("copy");
  }
  
  virtual Data * clone()
  {
    log("clone");
    return new TestData(*this);
  }
  
  static std::ostringstream sOut;
  static size_t sIdCounter;
  size_t        mId;
  
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
  
  // FIXME: is it possible to use a template for these two methods ?
  
  /** Return a const pointer to the data in the SmartPointer. */
  const TestData * data () const
  {
    return mPtr ? (TestData*)(mPtr->mDataPtr) : NULL;
  }
  
  /** Return a pointer to mutable data contained in the SmartPointer. Makes a copy if needed. */
  TestData * mutable_data ()
  {
    if (!mPtr)
      return NULL;

    if (mPtr->mRefCount > 1)
      copy();

    return (TestData*)(mPtr->mDataPtr);
  }
};

std::ostringstream TestData::sOut(std::ostringstream::out);
size_t TestData::sIdCounter = 0;

class ValueTest : public CxxTest::TestSuite
{  
public:
  void setUp()
  {
    TestData::sIdCounter = 0;
    TestData::sOut.str(std::string(""));
  }
  
  void test_create_destroy( void )
  {
    TestValue v1(new TestData);
    assert_log("[1] new");
    TestValue v2;
    v1 = v2;
    assert_log("[1] new, [1] dead");
  }
  
  void test_clone( void )
  {
    TestValue v1(new TestData);
    assert_log("[1] new");
    TestValue v2;
    v2 = v1;
    assert_log("[1] new");
    TestData * d;
    d = v1.mutable_data();
    assert_log("[1] new, [1] clone, [2] copy");
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
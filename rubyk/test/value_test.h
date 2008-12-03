// loop buffer test
// ordered_list_test.h 
#include "test_helper.h"

#define assert_log(x) _assert_log(__FILE__,__LINE__,x)

/** Define a simple Value for testing called TestValue. */
class TestData : public Data
{
public:
  TestData()
  {
    log("new");
  }
  
  TestData(const std::string& s)
  {
    if (sOut.str().size() > 0) sOut << ", ";
    sOut << "[" << mId << "] " << "new from \"" << s << "\"";
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

/** Define a simple Value for testing called TestValue. */
class TestValue : public Value
{
public:
  VALUE_METHODS(TestValue, TestData, NumberValue, Value)
};

std::ostringstream TestData::sOut(std::ostringstream::out);

class ValueTest : public ValueTestHelper
{  
public:
  
  virtual void setUp()
  {
    this->ValueTestHelper::setUp();
    TestData::sOut.str(std::string(""));
  }
  
  void test_create_destroy( void )
  {
    TestValue v1(new TestData);
    assert_log("[1] new");
    
    TestValue v2;
    // nothing created
    assert_log("");
    // deletes last reference on [1]
    v1 = v2;
    assert_log("[1] dead");
  }
  
  void test_from_string( void )
  {
    Value s("\"Hello\"");
    TS_ASSERT(s.is_string());
  }
  
  void test_clone( void )
  {
    TestValue v1(new TestData);
    assert_log("[1] new");
    TestValue v2;
    assert_ref_count(v1, 1);
    
    // increase reference count
    v2 = v1;
    assert_log("");
    // v1 and v2 share the same object
    assert_id(v1, 1);
    assert_id(v2, 1);
    assert_ref_count(v1, 2);
    assert_ref_count(v2, 2);
    
    const TestData * d;
    // const: no copy
    d = v1.data();
    assert_log("");
    // copy on write
    d = v1.mutable_data();
    assert_log("[1] clone, [2] copy");
  }
  
  void test_type_conversion( void )
  {
    TestValue v1(new TestData);
    assert_log("[1] new");
    assert_ref_count(v1, 1);
    
    // anonymize & share data
    Value sig(v1);
    assert_log("");
    assert_id(sig, 1);
    assert_ref_count(v1, 2);
    // type conversion & share data
    TestValue v3(sig);
    assert_log(""); // no object created
    assert_id(v3, 1);
    assert_ref_count(v1, 3);
  }
  
  void test_set( void )
  {
    TestValue v1(new TestData);
    assert_log("[1] new");
    assert_ref_count(v1, 1);
    
    // anonymize & share data
    Value sig(v1);
    assert_log("");
    assert_id(sig, 1);
    assert_ref_count(v1, 2);
    // type conversion & share data
    TestValue v3;
    TS_ASSERT(sig.set(v3));
    assert_log(""); // no object created
    assert_id(v3, 1);
    assert_ref_count(v3, 3);
    // make sure we can alter the object
    v3.mutable_data();
    assert_ref_count(v3, 1);
    assert_log("[1] clone, [2] copy");
  }
  
  void test_accessor( void )
  {
    TestValue v1(new TestData);
    assert_log("[1] new");
    assert_ref_count(v1, 1);
    
    // share data
    TestValue v2(v1);
    assert_log("");
    assert_id(v2, 1);
    assert_ref_count(v1, 2);
    
    // const accessor on shared data
    TS_ASSERT_EQUALS(v1->id(), 1);
    TS_ASSERT_EQUALS((*v1).id(), 1);
    assert_log("");
    assert_id(v2, 1);
    assert_ref_count(v1, 2);
    
    // non-const accessor on shared
    TS_ASSERT_EQUALS(v1.mutable_data()->id(), 2);
    assert_log("[1] clone, [2] copy");
    assert_id(v1, 2);
    assert_id(v2, 1);
    assert_ref_count(v1, 1);
    assert_ref_count(v2, 1);
    
    // non-const accessor on non-shared
    TS_ASSERT_EQUALS(v1.mutable_data()->id(), 2);
    assert_log("");
    assert_id(v1, 2);
    assert_ref_count(v1, 1);
  }
  
  void test_stream( void )
  {
    std::ostringstream out(std::ostringstream::out);
    Value v;
    out << v;
    TS_ASSERT_EQUALS(out.str(), "Nil");
  }
  
protected:
  void _assert_log(const char * file, int lineno, const char * pLog)
  {
    _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(std::string("log")), TestData::sOut.str(), std::string(pLog));
    TestData::sOut.str(std::string(""));
  }
};
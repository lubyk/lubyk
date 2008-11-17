
#define assert_log(x) _assert_log(__FILE__,__LINE__,x)
#define assert_ref_count(x,y) _assert_ref_count(__FILE__,__LINE__,x,y)
#define assert_id(x,y) _assert_id(__FILE__,__LINE__,x,y)

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
  
  virtual value_t type() const
  { return NumberValue; }
  
  size_t id() const
  { return mId; }

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
  VALUE_METHODS(TestValue, TestData, NumberValue)
  
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
  
  void _assert_ref_count(const char * file, int lineno, size_t count, Value& v)
  {
    _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(std::string("ref count")), count, v.ref_count());
  }
  
  void _assert_id(const char * file, int lineno, size_t id, Value& v)
  {
    _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(std::string("id")), id, ((TestValue)v).data_id());
  }
};
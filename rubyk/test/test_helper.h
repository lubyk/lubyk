#ifndef _TEST_HELPER_H_
#define _TEST_HELPER_H_
#include "globals.cpp"
#include "rubyk.h"
#include <sstream>

#define assert_log(x) _assert_log(__FILE__,__LINE__,x)
#define assert_ref_count(x,y) _assert_ref_count(__FILE__,__LINE__,x,y)
#define assert_id(x,y) _assert_id(__FILE__,__LINE__,x,y)
#define assert_matrix_equal(x,y) _assert_matrix_equal(__FILE__,__LINE__,x,y)

#define assert_print(x,y) _assert_print(__FILE__,__LINE__,x,y)
#define assert_result(x,y) _assert_result(__FILE__,__LINE__,x,y)
#define assert_inspect(x,y) _assert_inspect(__FILE__,__LINE__,x,y)
#define assert_info(x,y) _assert_inspect(__FILE__,__LINE__,x,y)
#define create(a,b,c,d) _create(__FILE__,__LINE__,a,b,c,d)
#define assert_call(a,b,c) _assert_call(__FILE__,__LINE__,a,b,c)

#define ___ERK_ASSERT_EQUALS(f,l,cmd,x,y,m) CxxTest::doAssertEquals( (f), (l), (cmd), (x), #y, (y), (m) )
#define ___RK_ASSERT_EQUALS(f,l,cmd,x,y,m) { _TS_TRY { ___ERK_ASSERT_EQUALS(f,l,cmd,x,y,m); } __TS_CATCH(f,l) }
#define _RK_ASSERT_EQUALS(f,l,cmd,x,y) ___RK_ASSERT_EQUALS(f,l,cmd,x,y,0)

// ========================== Values ====================== //

/** Helper class to test Values. */
class ValueTestHelper : public CxxTest::TestSuite
{
public:
  
  void setUp()
  {
    Data::sShowId    = true;
    Data::sIdCounter = 0;
  }
  
protected:
  void _assert_ref_count(const char * file, int lineno, const Value& v, size_t count)
  {
    _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(std::string("ref count")), v.ref_count(), count);
  }
  
  void _assert_id(const char * file, int lineno, const Value& v, size_t id)
  {
    _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(std::string("id")), v.data_id(), id);
  }
};

// ========================== Nodes  ====================== //

/** Helper class to test Nodes. */
class NodeTestHelper : public CxxTest::TestSuite
{
public:
  
  void setUp()
  {
    Data::sIdCounter = 0;
    Data::sShowId    = true;
    mRoot.clear();
    mRoot.classes()->set_lib_path("lib");
  }
  
protected:
  Root mRoot;
  
  void _create(const char * file, int lineno, const char * pUrl, const char * pClass, const char* pParams, const char* pResult)
  {
    Value res = mRoot.new_object(pUrl, pClass, pParams);
    _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(std::string("create")),
                       res.to_string(), std::string(pResult));
  }
  
  void _assert_call(const char * file, int lineno, const char * pUrl, const char * pVal, const char * pResult)
  {
    Value res = mRoot.call(pUrl, Value(pVal));
    std::string str = res.is_string() ? String(res).string() : res.to_string();
    _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(std::string(pUrl)), str, std::string(pResult));
  }
  
  void _assert_call(const char * file, int lineno, const char * pUrl, const char * pVal, real_t pResult)
  {
    Value res = mRoot.call(pUrl, Value(pVal));
    Number n(res);
    if (n.is_nil()) {
      _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(std::string(pUrl)), res.to_string(), TS_AS_STRING(pResult));
    } else {
      _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(std::string(pUrl)), n.value(), pResult);
    }
  }
  
  void _assert_inspect(const char * file, int lineno, const char * pName, const char * pInfo)
  {
    std::string url = std::string(pName).append("/#inspect");
    Value res = mRoot.call(url);
    std::string str = res.is_string() ? String(res).string() : res.to_string();
    _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(url), str, std::string(pInfo));
  }
  
  
  void _assert_inspect(const char * file, int lineno, const char * pName, real_t pInfo)
  {
    std::string url = std::string(pName).append("/#inspect");
    Value res = mRoot.call(url);
    Number n(res);
    if (n.is_nil()) {
      _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(url), res.to_string(), TS_AS_STRING(pInfo));
    } else {
      _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(url), n.value(), pInfo);
    }
  }
  
  void _assert_info(const char * file, int lineno, const char * pName, const char * pInfo)
  {
    std::string url = std::string(pName).append("/#info");
    Value res = mRoot.call(url);
    std::string str = res.is_string() ? String(res).string() : res.to_string();
    _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(url), str, std::string(pInfo));
  }
};

class DummyNumber : public Object
{
public:
  DummyNumber(const char * pName, int pCounter) : Object(pName), mCounter(pCounter) {}
  
  virtual const Value trigger (const Value& val)
  {
    mCounter = val;
    return mCounter;
  }
  
  Number mCounter;
};


// ========================== ParseHelper  ====================== //

class ParseHelper : public CxxTest::TestSuite
{
public:
  ParseHelper() : mOutput(std::ostringstream::out), mInput(std::istringstream::in)
  { 
    mCmd  = new Command(mInput, mOutput);
    mCmd->set_server(&mRoot);
  }
  
  void setUp()
  {
    Data::sIdCounter = 0;
    Data::sShowId    = false;
    mRoot.clear();
    mRoot.classes()->set_lib_path("lib");
    mOutput.str(std::string("")); // clear output
  }
  
protected:
  Root mRoot;
  Command * mCmd;
  std::ostringstream mOutput;
  std::istringstream mInput;
  
protected:
  
//FIX  void setup_with_print(const char* pInput)
//FIX  {
//FIX    clean_start();
//FIX    mRoot.unlock();
//FIX      mCmd->parse("p=Print()\nn=>p\n");
//FIX      mCmd->parse(pInput);
//FIX      mOutput.str(std::string("")); // clear output
//FIX    mRoot.lock();
//FIX  }
//FIX  
//FIX  void clean_assert_result(const char * pInput, const char * pOutput)
//FIX  {
//FIX    clean_start();
//FIX    assert_result(pInput, pOutput);
//FIX  }

  void _assert_result(const char * file, int lineno, const char * pInput, const char * pOutput)
  {
    mOutput.str(std::string("")); // clear output
    mRoot.unlock();
      mCmd->parse(pInput);
    mRoot.lock();
    _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(std::string(pInput)), mOutput.str(), std::string(pOutput));
  }
  
//FIX  void clean_assert_bang(const char * pInput, const char * pOutput)
//FIX  {
//FIX    setup_with_print(pInput);
//FIX    assert_bang("\n", pOutput);
//FIX  }
//FIX  
//FIX  void assert_bang(const char * pInput, const char * pOutput)
//FIX  { 
//FIX    mRoot.unlock();
//FIX      mOutput.str(std::string("")); // clear output
//FIX      mCmd->set_silent(true);
//FIX      mCmd->parse(pInput);
//FIX      mCmd->parse("\nn.bang\n");
//FIX      mCmd->set_silent(false);
//FIX    mRoot.lock();
//FIX    TS_ASSERT_EQUALS( mOutput.str(), std::string(pOutput));
//FIX  }
//FIX  
//FIX  void clean_assert_print(const char * pInput, const char * pOutput)
//FIX  {
//FIX    clean_start();
//FIX    assert_print(pInput, pOutput);
//FIX    
//FIX  }
  
  void _assert_print(const char * file, int lineno, const char * pInput, const char * pOutput)
  { 
    mRoot.unlock();
      mOutput.str(std::string("")); // clear output
      mCmd->set_silent(true);
      mCmd->parse(pInput);
      mCmd->set_silent(false);
    mRoot.lock();
    //mRoot.run(); // loop once
    _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(std::string(pInput)), mOutput.str(), std::string(pOutput));
  }
  
//FIX  void clean_assert_run(time_t pLength, const char * pInput, const char * pOutput)
//FIX  {
//FIX    clean_start();
//FIX    assert_run(pLength, pInput, pOutput);
//FIX  }
//FIX  void assert_run(time_t pLength, const char * pInput, const char * pOutput)
//FIX  {
//FIX    time_t start;
//FIX    mRoot.unlock();
//FIX      mCmd->parse("print=Print()\n");
//FIX    mRoot.lock();
//FIX    mOutput.str(std::string("")); // clear output
//FIX    mInput.str(std::string(pInput)); // set input
//FIX    mCmd->set_silent(true);
//FIX    mRoot.listen_to_command(*mCmd);
//FIX    start = mRoot.mCurrentTime;
//FIX    while(mRoot.mCurrentTime <= start + pLength && mRoot.run()) {
//FIX      ;
//FIX    }
//FIX    mCmd->set_silent(false);
//FIX    
//FIX    TS_ASSERT_EQUALS( mOutput.str(), std::string(pOutput));
//FIX  }
};

#endif // _TEST_HELPER_H_
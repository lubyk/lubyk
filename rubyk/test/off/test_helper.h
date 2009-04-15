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
#define assert_run(x,y) _assert_run(__FILE__,__LINE__,x,y)
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
  void _assert_ref_count(const char * file, int lineno, const Value &v, size_t count)
  {
    _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(std::string("ref count")), v.ref_count(), count);
  }
  
  void _assert_id(const char * file, int lineno, const Value &v, size_t id)
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
    worker_.clear();
    worker_.classes()->set_lib_path("lib");
  }
  
protected:
  Worker worker_;
  
  void _create(const char * file, int lineno, const char * url, const char * class_name, const char* params, const char* pResult)
  {
    Value res = worker_.new_object(url, class_name, params);
    _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(std::string("create")),
                       res.to_json(), std::string(pResult));
  }
  
  void _assert_call(const char * file, int lineno, const char * url, const char * pVal, const char * pResult)
  {
    Value res = worker_.call(url, Value(pVal));
    std::string str = res.is_string() ? String(res).string() : res.to_json();
    _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(std::string(url)), str, std::string(pResult));
  }
  
  void _assert_call(const char * file, int lineno, const char * url, const char * pVal, Real pResult)
  {
    Value res = worker_.call(url, Value(pVal));
    Number n(res);
    if (n.is_nil()) {
      _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(std::string(url)), res.to_json(), TS_AS_STRING(pResult));
    } else {
      _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(std::string(url)), n.value(), pResult);
    }
  }
  
  void _assert_inspect(const char * file, int lineno, const char * pName, const char * pInfo)
  {
    std::string url = std::string(pName).append("/#inspect");
    Value res = worker_.call(url);
    std::string str = res.is_string() ? String(res).string() : res.to_json();
    _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(url), str, std::string(pInfo));
  }
  
  
  void _assert_inspect(const char * file, int lineno, const char * pName, Real pInfo)
  {
    std::string url = std::string(pName).append("/#inspect");
    Value res = worker_.call(url);
    Number n(res);
    if (n.is_nil()) {
      _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(url), res.to_json(), TS_AS_STRING(pInfo));
    } else {
      _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(url), n.value(), pInfo);
    }
  }
  
  void _assert_info(const char * file, int lineno, const char * pName, const char * pInfo)
  {
    std::string url = std::string(pName).append("/#info");
    Value res = worker_.call(url);
    std::string str = res.is_string() ? String(res).string() : res.to_json();
    _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(url), str, std::string(pInfo));
  }
};

class DummyNumber : public oscit::Object
{
public:
  DummyNumber(const char * pName, int pCounter) : oscit::Object(pName), mCounter(pCounter) {}
  
  virtual ~DummyNumber() {}
  
  virtual const Value trigger (const Value &val)
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
  ParseHelper() : output_(std::ostringstream::out), input_(std::istringstream::in)
  { 
    mCmd  = new Command(input_, output_);
    mCmd->set_worker(&worker_);
  }
  
  ~ParseHelper()
  {
    delete mCmd;
  }
  
  void setUp()
  {
    Data::sIdCounter = 0;
    Data::sShowId    = false;
    worker_.clear();
    worker_.classes()->set_lib_path("lib");
    output_.str(std::string("")); // clear output
  }
  
protected:
  Worker worker_;
  Command * mCmd;
  std::ostringstream output_;
  std::istringstream input_;
  
protected:
  
  void setup_with_print(const char* pInput)
  {
    Node * print;
    worker_.unlock();
      mCmd->parse("p=Print()\nn=>p\n");
      print = TYPE_CAST(Node, worker_.find("/p"));
      if (print) mCmd->observe(print);
      mCmd->parse(pInput);
      output_.str(std::string("")); // clear output
    worker_.lock();
  }
  
//FIX  void clean_assert_result(const char * pInput, const char * pOutput)
//FIX  {
//FIX    clean_start();
//FIX    assert_result(pInput, pOutput);
//FIX  }

  void _assert_result(const char * file, int lineno, const char * pInput, const char * pOutput)
  {
    output_.str(std::string("")); // clear output
    worker_.unlock();
      mCmd->parse(pInput);
    worker_.lock();
    _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(std::string(pInput)), output_.str(), std::string(pOutput));
  }
  
//FIX  void clean_assert_bang(const char * pInput, const char * pOutput)
//FIX  {
//FIX    setup_with_print(pInput);
//FIX    assert_bang("\n", pOutput);
//FIX  }
//FIX  
//FIX  void assert_bang(const char * pInput, const char * pOutput)
//FIX  { 
//FIX    worker_.unlock();
//FIX      output_.str(std::string("")); // clear output
//FIX      mCmd->set_silent(true);
//FIX      mCmd->parse(pInput);
//FIX      mCmd->parse("\nn.bang\n");
//FIX      mCmd->set_silent(false);
//FIX    worker_.lock();
//FIX    TS_ASSERT_EQUALS( output_.str(), std::string(pOutput));
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
    worker_.unlock();
      output_.str(std::string("")); // clear output
      mCmd->set_silent(true);
      mCmd->parse(pInput);
      mCmd->set_silent(false);
    worker_.lock();
    
    _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(std::string(pInput)), output_.str(), std::string(pOutput));
  }
  
//FIX  void clean_assert_run(time_t pLength, const char * pInput, const char * pOutput)
//FIX  {
//FIX    clean_start();
//FIX    assert_run(pLength, pInput, pOutput);
//FIX  }

  void _assert_run(const char * file, int lineno, time_t pLength, const char * pOutput)
  {
    time_t start = worker_.current_time_;
    output_.str(std::string("")); // clear output
    mCmd->set_silent(true);
    while(worker_.current_time_ <= start + pLength && worker_.do_run())
      ;
    mCmd->set_silent(false);
    _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING("running"), output_.str(), std::string(pOutput));
  }
};

#endif // _TEST_HELPER_H_
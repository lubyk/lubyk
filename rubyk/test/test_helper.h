#ifndef _VALUE_TEST_HELPER_H_
#define _VALUE_TEST_HELPER_H_
#include "globals.cpp"
#include "rubyk.h"
#include <sstream>

#define assert_log(x) _assert_log(__FILE__,__LINE__,x)
#define assert_ref_count(x,y) _assert_ref_count(__FILE__,__LINE__,x,y)
#define assert_id(x,y) _assert_id(__FILE__,__LINE__,x,y)
#define assert_matrix_equal(x,y) _assert_matrix_equal(__FILE__,__LINE__,x,y)

#define assert_print(x,y) _assert_print(__FILE__,__LINE__,x,y)
#define assert_inspect(x,y) _assert_inspect(__FILE__,__LINE__,x,y)
#define assert_info(x,y) _assert_inspect(__FILE__,__LINE__,x,y)
#define create(a,b,c,d) _create(__FILE__,__LINE__,a,b,c,d)
#define assert_call(a,b,c) _assert_call(__FILE__,__LINE__,a,b,c)

#define ___ERK_ASSERT_EQUALS(f,l,cmd,x,y,m) CxxTest::doAssertEquals( (f), (l), (cmd), (x), #y, (y), (m) )
#define ___RK_ASSERT_EQUALS(f,l,cmd,x,y,m) { _TS_TRY { ___ERK_ASSERT_EQUALS(f,l,cmd,x,y,m); } __TS_CATCH(f,l) }
#define _RK_ASSERT_EQUALS(f,l,cmd,x,y) ___RK_ASSERT_EQUALS(f,l,cmd,x,y,0)

// ========================== Values ====================== //

size_t Data::sIdCounter = 0;

/** Helper class to test Values. */
class ValueTestHelper : public CxxTest::TestSuite
{
public:
  
  void setUp()
  {
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
    mRoot.clear();
    mRoot.classes()->set_lib_path("lib");
  }
  
protected:
  Root mRoot;
  
  void _create(const char * file, int lineno, const char * pName, const char * pClass, const char* pParams, const char* pUrl)
  {
    Hash h;
    h.set_key("url", String(pName));
    h.set_key("params", Hash(pParams));
    Value res = mRoot.classes()->new_node(pName, pClass, h);
    _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(std::string("create")),
                       res.to_string(), std::string(pUrl));
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

#endif // _VALUE_TEST_HELPER_H_
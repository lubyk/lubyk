#ifndef _VALUE_TEST_HELPER_H_
#define _VALUE_TEST_HELPER_H_
#include "globals.cpp"
#include "class.h"
#include <sstream>

#define assert_log(x) _assert_log(__FILE__,__LINE__,x)
#define assert_ref_count(x,y) _assert_ref_count(__FILE__,__LINE__,x,y)
#define assert_id(x,y) _assert_id(__FILE__,__LINE__,x,y)
#define assert_matrix_equal(x,y) _assert_matrix_equal(__FILE__,__LINE__,x,y)

#define assert_print(x,y) _assert_print(__FILE__,__LINE__,x,y)
#define assert_inspect(x,y) _assert_inspect(__FILE__,__LINE__,x,y)
#define create(x,y,z) _create(__FILE__,__LINE__,x,y,z)

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
  }
  
protected:
  void _create(const char * file, int lineno, const char * pClass, const char * pName, const char* pParams)
  {
    Hash h;
    h.set_key("url", String(pName));
    h.set_key("params", Hash(pParams));
    Value res = Object::call(std::string(CLASS_ROOT).append("/").append(pClass).append("/new"), h);
    _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(std::string("create")),
                       res.to_string(), 
                       std::string("\"/").append(pName).append("\""));
  }
  
  void _assert_inspect(const char * file, int lineno, const char * pName, const char * pInfo)
  {
    _RK_ASSERT_EQUALS( file, lineno, TS_AS_STRING(std::string("inspect")),
                       Object::call(std::string("/").append(pName).append("/#inspect")).to_string(), 
                       std::string("\"").append(std::string(pInfo)).append("\""));
  }
};

#endif // _VALUE_TEST_HELPER_H_
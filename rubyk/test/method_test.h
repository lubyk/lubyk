// ordered_list_test.h 
#include <cxxtest/TestSuite.h>
#include "method.h"
#include "test_helper.h"

// FIXME: !!!! destroy methods on object destroy !!!!

class Person
{
public:
  Person(const char * pName) : mName(pName) {}
  
  static const Value class_method(Root& root, const Value& val)
  {
    return val.is_string() ? val : Error("Not a string");
  }
  
  const Value name(const Value& val)
  {
    val.set(mName);
    return mName;
  }
  
  String mName;
};

class MethodTest : public ValueTestHelper
{
public:
  void test_trigger_class_method( void )
  {
    Root root;
    Object * cm = root.adopt(new ClassMethod("hello", &Person::class_method));
    Value res;
    Object * obj;
    TS_ASSERT_EQUALS(cm->url(), "/hello");
    TS_ASSERT(root.get(&obj, "/hello"));
    
    res = obj->trigger(Bang(true)); // [1]
    TS_ASSERT_EQUALS(res.to_string(), "[2] #Not a string");
    
    res = obj->trigger(String("Yoba"));
    TS_ASSERT_EQUALS(res.to_string(), "[3] Yoba");
  }
  
  void test_trigger_method( void )
  {
    Root root;
    Person p("Paul"); // [1]
    root.adopt(new Method("paul_name", &p, &Method::cast_method<Person, &Person::name>));
    Value res;
    Object * obj;
    TS_ASSERT(root.get(&obj, "/paul_name"));
    
    res = obj->trigger(gNilValue); 
    TS_ASSERT_EQUALS(res.to_string(), "[1] Paul");
    
    res = obj->trigger(String("John")); // [2]
    TS_ASSERT_EQUALS(res.to_string(), "[2] John");
    
    TS_ASSERT_EQUALS(p.mName.string(), "John");
  }
};
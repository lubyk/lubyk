/*
  ==============================================================================

   This file is part of the OSCIT library (http://rubyk.org/liboscit)
   Copyright (c) 2007-2009 by Gaspard Bucher - Buma (http://teti.ch).

  ------------------------------------------------------------------------------

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

  ==============================================================================
*/

#include "test_helper.h"

/** Methods and ClassMethods can also point on objects that are not descendants of the Object. */
class Person : public Object
{
 public: 
  /** Class signature. */
  TYPED("Object.Person")
  
  Person(const char * name) : Object(name) {}
  
  /** A simple class method. */
  static const Value class_method(Root *root, const Value &val) {
    return Value(std::string(val.to_json()));
  }
  
  /** A simple member method. */
  const Value name(const Value &val) {
    if (val.is_string()) set_name(val.str());
    return Value(std::string(name_).append(" On Earth"));
  }
  
  const std::string &name() const {
    return name_;
  }
};

class Employee : public Person {
public:
  Employee(const char *name) : Person(name) {}
};

class MethodTest : public TestHelper
{
public:
  void test_trigger_class_method( void ) {
    Root root;
    root.adopt(new ClassMethod("hello", &Person::class_method, StringIO("", "Send me strings only.")));
    Value res;
    
    res = root.call("/hello", MatrixValue(2,3));
    
    assert_equal("\"400 \'/hello\' (Send me strings only.).\"", res.to_json());
    
    res = root.call("/hello");
    
    assert_true(res.is_string());
    assert_equal("null", res.str());
  }
  
  void test_trigger_method( void ) {
    Root root;
    Person * eva = root.adopt(new Person("Eva"));
    eva->adopt(new Method(eva, "name", &Method::cast_method<Person, &Person::name>, StringIO("", "Name of the person.")));
    Value res;
    
    res = root.call("/Eva/name");
    assert_equal("Eva On Earth", res.str());
    
    res = root.call("/Eva/name", Value("Lilith"));
    assert_equal("Lilith On Earth", res.str());
    
    res = root.call("/Eva/name");
    assert_true(res.is_error());
    assert_equal(NOT_FOUND_ERROR, res.error_code());
    
    assert_equal("Lilith", eva->name());
  }
  
  void test_trigger_method_tmethod_syntax( void ) {
    Root root;
    Person * eva = root.adopt(new Person("Eva"));
    // template syntax is nicer and more readable
    eva->adopt(new TMethod<Person, &Person::name>(eva, "name", StringIO("", "Name of the person.")));
    Value res;
    
    res = root.call("/Eva/name");
    assert_equal("Eva On Earth", res.str());
    
    res = root.call("/Eva/name", Value("Lilith"));
    assert_equal("Lilith On Earth", res.str());
    
    res = root.call("/Eva/name");
    assert_true(res.is_error());
    assert_equal(NOT_FOUND_ERROR, res.error_code());
    
    assert_equal("Lilith", eva->name());
  }
  
  void test_cast_super_method( void ) {
    Root root;
    Employee *joe = root.adopt(new Employee("Joe"));
    joe->adopt(new Method(joe, "name", &Method::cast_method<Employee, Person, &Person::name>, StringIO("", "Name of the person.")));
    Value res;
    
    res = root.call("/Joe/name");
    assert_equal("Joe On Earth", res.str());
    
    res = root.call("/Joe/name", Value("Tarzan"));
    assert_equal("Tarzan On Earth", res.str());
    
    res = root.call("/Joe/name");
    assert_true(res.is_error());
    assert_equal(NOT_FOUND_ERROR, res.error_code());
    
    assert_equal("Tarzan", joe->name());
  }
};
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

class MethodTest : public TestHelper
{
public:
  void test_trigger_class_method( void ) {
    Root root;
    root.adopt(new ClassMethod("hello", &Person::class_method, H("s"), "Send me strings only."));
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
    eva->adopt(new Method(eva, "name", &Method::cast_method<Person, &Person::name>, H("s"), "Name of the person."));
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
    eva->adopt(new TMethod<Person, &Person::name>(eva, "name", H("s"), "Name of the person."));
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
};
#include "test_helper.h"
#include "oscit/mapper.h"

class MapperTest : public TestHelper
{
public:
  void test_simple_parse( void ) {
    Mapper mapper;
    assert_true(mapper.parse(std::string("/slider1/1 [0,1] --> /m/tempo [60,240]")));
    std::string target;
    Real value;
    assert_true(mapper.map("/slider1/1", 0.5, &target, &value));
    assert_equal("/m/tempo", target);
    assert_equal(150.0, value);
  }
  
  void test_parse_empty_lines( void ) {
    Mapper mapper;
    assert_true(mapper.parse(std::string("\n\n/slider1/1 [0,1] --> /m/tempo [60,240]\n\n/foo/bar [1,2] -> /baz [10, 20]")));
    std::string target;
    Real value;
    assert_true(mapper.map("/foo/bar", 0.5, &target, &value));
    assert_equal("/baz", target);
    assert_equal(10.0, value);
  }
  
  void test_parse_return_in_definition( void ) {
    Mapper mapper;
    assert_true(mapper.parse(std::string("\n\n/slider1/1 [0,1]\n--> /m/tempo [60,240]\n\n/foo/bar [1,2] -> /baz [10, 20]")));
    std::string target;
    Real value;
    assert_true(mapper.map("/slider1/1", 1.5, &target, &value));
    assert_equal("/m/tempo", target);
    assert_equal(240.0, value);
  }
  
  void test_parse_comment( void ) {
    Mapper mapper;
    assert_true(mapper.parse(std::string("\n\n/slider1/1 [0,1] --> /m/tempo [60,240]\n# comment\n/foo/bar [1,2] -> /baz [10, 20]")));
    std::string target;
    Real value;
    assert_true(mapper.map("/slider1/1", 1.5, &target, &value));
    assert_equal("/m/tempo", target);
    assert_equal(240.0, value);
  }
  
  void test_parse_comment_in_definition( void ) {
    Mapper mapper(10);
    assert_true(mapper.parse(std::string("\n\n/slider1/1 [0,1] #foo bar\n -->#baz\n /m/tempo #blah\n[60,240]\n# comment\n/foo/bar [1,2] -> /baz [10, 20]")));
    std::string target;
    Real value;
    assert_true(mapper.map("/slider1/1", 1.5, &target, &value));
    assert_equal("/m/tempo", target);
    assert_equal(240.0, value);
  }
};
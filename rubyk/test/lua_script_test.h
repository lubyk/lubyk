#include <stdio.h>
#include <string>

#include "test_helper.h"
#include "lua_script.h"

class LuaScriptTest : public TestHelper {
public:
  virtual void setUp() {
    planet_.clear();
    script_ = planet_.adopt(new LuaScript);
    script_->set_name("lua");
  }
  
  void tearDown() {
    delete script_;
  }
  
  void test_compile( void ) {
    Value res = parse("function foo()\nend");
    assert_true(res.is_string());
    assert_equal("function foo()\nend", res.str());
    assert_true(script_->is_ok());
  }
  
  void test_compilation_error( void ) {
    Value res = parse("function foo\nend");
    assert_true(res.is_error());
    assert_equal(BAD_REQUEST_ERROR, res.error_code());
    assert_equal("[string \"lua\"]:2: \'(\' expected near \'end\'.", res.error_message());
    assert_false(script_->is_ok());
  }
  
  void test_runtime_error( void ) {
    Value res = parse("x = y + 3"); // y undefined
    assert_true(res.is_error());
    assert_equal(BAD_REQUEST_ERROR, res.error_code());
    assert_equal("[string \"lua\"]:1: attempt to perform arithmetic on global 'y' (a nil value).", res.error_message());
    assert_false(script_->is_ok());
  }
  
  void test_add_inlet( void ) {
    script_->adopt(new Object("in")); // this is done by Class normally
    Value res = parse("inlet('tempo', {0.0, 'bpm', 'Main beat machine tempo.'})");
    assert_true(res.is_string());
    Object *inlet = planet_.object_at("/lua/in/tempo");
    assert_true(inlet != NULL);
    assert_equal("fss", inlet->type().type_tag());
    assert_equal(0.0, inlet->type()[0].r);
    assert_equal("bpm", inlet->type()[1].str());
    assert_equal("Main beat machine tempo.", inlet->type()[2].str());
    
    res = parse("inlet('tempo', {0.0, 'bpm', 'Main beat machine tempo.'})");
    assert_true(res.is_string()); // no error
  }
  
  void test_rubyk_lua( void ) {
    // RealIO('bpm', 'Main beat machine tempo.')
  }
  
  
  
private:
  const Value parse(const char *string) {
    return script_->script(Value(string));
  }
  
  Planet    planet_;
  LuaScript *script_;
};
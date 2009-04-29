#include <stdio.h>
#include <string>

#include "test_helper.h"
#include "lua_script.h"

class LuaScriptTest : public TestHelper {
public:
  virtual void setUp() {
    planet_ = new Planet();
    planet_->call(LIB_URL, Value("lib"));
    script_ = planet_->adopt(new LuaScript);
    // all this is done by Class normally
    script_->set_name("lua");
    script_->adopt(new Object("in"));
    Value res = script_->init();
    if (res.is_error()) {
      std::cout << "Could not init LuaScript !\n" << res << "\n";
    }
  }
  
  void tearDown() {
    if (planet_) delete planet_;
    planet_ = NULL;
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
    Value res = parse("inlet('tempo', {0.0, 'bpm', 'Main beat machine tempo.'})");
    std::cout << res << "\n\n";
    assert_true(res.is_string());
    Object *inlet = planet_->object_at("/lua/in/tempo");
    assert_true(inlet != NULL);
    assert_equal("fss", inlet->type().type_tag());
    assert_equal(0.0, inlet->type()[0].r);
    assert_equal("bpm", inlet->type()[1].str());
    assert_equal("Main beat machine tempo.", inlet->type()[2].str());
    
    res = parse("inlet('tempo', {0.0, 'bpm', 'Main beat machine tempo.'})");
    assert_true(res.is_string()); // no error
  }
  
  void test_add_inlet_RealIO( void ) {
    // also tests loading of rubyk.lua
    Value res = parse("inlet('tempo', RealIO('bpm', 'Main beat machine tempo.'))");
    std::cout << res << "\n";
    assert_true(res.is_string());
    Object *inlet = planet_->object_at("/lua/in/tempo");
    assert_true(inlet != NULL);
    assert_equal("fss", inlet->type().type_tag());
    assert_equal(0.0, inlet->type()[0].r);
    assert_equal("bpm", inlet->type()[1].str());
    assert_equal("Main beat machine tempo.", inlet->type()[2].str());
  }
  
  void test_add_inlet_NilIO( void ) {
    // also tests loading of rubyk.lua
    Value res = parse("inlet('boom', NilIO('Ping pong.'))");
    assert_true(res.is_string());
    Object *inlet = planet_->object_at("/lua/in/boom");
    assert_true(inlet != NULL);
    assert_equal("Ns", inlet->type().type_tag());
    assert_equal("Ping pong.", inlet->type()[1].str());
  }
  
private:
  const Value parse(const char *string) {
    return script_->script(Value(string));
  }
  
  Planet    *planet_;
  LuaScript *script_;
};
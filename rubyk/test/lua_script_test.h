#include <stdio.h>
#include <string>

#include "test_helper.h"
#include "lua_script.h"

class LuaScriptTest : public TestHelper {
public:
  void test_compile( void ) {
    Planet base;
    LuaScript *script = base.adopt(new LuaScript);
    script->set_name("l1");
    Value res = script->script(Value("function foo()\nend"));
    assert_true(res.is_string());
    assert_equal("function foo()\nend", res.str());
    assert_true(script->is_ok());
  }
  
  void test_compilation_error( void ) {
    Planet base;
    LuaScript *script = base.adopt(new LuaScript);
    script->set_name("l1");
    Value res = script->script(Value("function foo\nend"));
    assert_true(res.is_error());
    assert_equal(BAD_REQUEST_ERROR, res.error_code());
    assert_equal("[string \"l1\"]:2: \'(\' expected near \'end\'.", res.error_message());
    assert_false(script->is_ok());
  }
  
  void test_runtime_error( void ) {
    Planet base;
    LuaScript *script = base.adopt(new LuaScript);
    script->set_name("l1");
    Value res = script->script(Value("x = y + 3")); // y undefined
    assert_true(res.is_error());
    assert_equal(BAD_REQUEST_ERROR, res.error_code());
    assert_equal("[string \"l1\"]:1: attempt to perform arithmetic on global 'y' (a nil value).", res.error_message());
    assert_false(script->is_ok());
  }
};
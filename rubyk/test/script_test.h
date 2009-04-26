#include <stdio.h>
#include <string>

#include "test_helper.h"
#include "mock/dummy_script.h"

class ScriptTest : public TestHelper {
public:
  void test_script( void ) {
    Planet base;
    DummyScript *script = base.adopt(new DummyScript);
    Value res = script->script(Value("This script will compile fine."));
    assert_true(res.is_string());
    assert_equal("This script will compile fine.", res.str());
    assert_true(script->is_ok());
    res.set_nil();
    res = script->script(gNilValue);
    assert_true(res.is_string());
    assert_equal("This script will compile fine.", res.str());
  }
  
  void test_bad_script( void ) {
    Planet base;
    DummyScript *script = base.adopt(new DummyScript);
    Value res = script->script(Value("This script will raise a [compilation error]."));
    assert_true(res.is_error());
    assert_equal(BAD_REQUEST_ERROR, res.error_code());
    assert_equal("Compilation error near ' a [compilation error].'.", res.error_message());
    assert_false(script->is_ok());
    res.set_nil();
    res = script->script(gNilValue);
    assert_true(res.is_string());
    assert_equal("This script will raise a [compilation error].", res.str());
    // compile a correct script
    res = script->script(Value("This script will compile fine."));
    assert_true(res.is_string());
    assert_equal("This script will compile fine.", res.str());
    assert_true(script->is_ok());
  }
  
  void test_file( void ) {
    Planet base;
    DummyScript *script = base.adopt(new DummyScript);
    Value res = script->file(Value("test/fixtures/script_test_file.txt"));
    assert_true(res.is_string());
    assert_equal("test/fixtures/script_test_file.txt", res.str());
    assert_true(script->is_ok());
    res.set_nil();
    res = script->file(gNilValue);
    assert_true(res.is_string());
    assert_equal("test/fixtures/script_test_file.txt", res.str());
    res = script->script(gNilValue);
    assert_equal("On a dark, rainy day,\nWe will go out to fish.\nWith the elements we play\nAnd fishes end served on a dish.\n", res.str());
  }
  
  void test_change_content_should_be_saved_in_file( void ) {
    Planet base;
    DummyScript *script = base.adopt(new DummyScript);
    Value res = script->file(Value("test/fixtures/script_test_new_file.txt"));
    assert_true(res.is_error());
    assert_equal(BAD_REQUEST_ERROR, res.error_code());
    assert_equal("Could not stat 'test/fixtures/script_test_new_file.txt'.", res.error_message());
    assert_false(script->is_ok());
    res = script->script(gNilValue);
    assert_true(res.is_string());
    assert_equal("", res.str());
    
    res = script->script(Value("Foo bar baz !"));
    assert_true(res.is_string());
    assert_equal("Foo bar baz !", res.str());
    
    std::ifstream in("test/fixtures/script_test_new_file.txt" ,std::ios::in);
      std::ostringstream oss;
      oss << in.rdbuf();
    in.close();
    assert_equal("Foo bar baz !", oss.str());
    
    remove("test/fixtures/script_test_new_file.txt");
  }
  
  void test_file_not_found( void ) {
    Planet base;
    DummyScript *script = base.adopt(new DummyScript);
    Value res = script->file(Value("test/fixtures/script_test_file_not_found.txt"));
    assert_true(res.is_error());
    assert_equal(BAD_REQUEST_ERROR, res.error_code());
    assert_equal("Could not stat \'test/fixtures/script_test_file_not_found.txt\'.", res.error_message());
    assert_false(script->is_ok());
    res = script->file(gNilValue);
    assert_true(res.is_string());
    assert_equal("test/fixtures/script_test_file_not_found.txt", res.str());
    // load another existing file
    res = script->file(Value("test/fixtures/script_test_file.txt"));
    assert_true(res.is_string());
    assert_equal("test/fixtures/script_test_file.txt", res.str());
    assert_true(script->is_ok());
  }
  
  void test_bad_file( void ) {
    Planet base;
    DummyScript *script = base.adopt(new DummyScript);
    Value res = script->file(Value("test/fixtures/script_test_bad_file.txt"));
    assert_true(res.is_error());
    assert_equal(BAD_REQUEST_ERROR, res.error_code());
    assert_equal("Compilation error near \'he [compilation error] \'.", res.error_message());
    assert_false(script->is_ok());
    res.set_nil();
    res = script->file(gNilValue);
    assert_true(res.is_string());
    assert_equal("test/fixtures/script_test_bad_file.txt", res.str());
    assert_equal("On a dark, rainy day,\nWe will go out to fish.\nWith the [compilation error] #)IOYLNK$\n#:L)(_)*@#)(()}#\n#\n#&#()&U)$", script->script(gNilValue).str());
    // load a correct file
    res = script->file(Value("test/fixtures/script_test_file.txt"));
    assert_true(res.is_string());
    assert_equal("test/fixtures/script_test_file.txt", res.str());
    assert_true(script->is_ok());
  }
  
  void test_reload( void ) {
    Planet base;
    DummyScript *script = base.adopt(new DummyScript);
    script->file(Value("test/fixtures/script_test_reload.txt"));
    
    assert_equal(1.0, script->reload(gNilValue).r);
    Value res = script->reload(Value(0.001)); // 1 [ms]
    
    std::ofstream out("test/fixtures/script_test_reload.txt", std::ios::out);
      out << "This is a nice script.";
    out.close();
    
    // too soon, should not reload
    script->reload_script();
    assert_false(script->is_ok());
    assert_equal("", script->script(gNilValue).str());
    
    base.worker()->current_time_ = 1;
    // reload
    
    script->reload_script();
    assert_true(script->is_ok());
    assert_equal("This is a nice script.", script->script(gNilValue).str());
    
    remove("test/fixtures/script_test_reload.txt");
  }
  
  void test_inspect( void ) {
    Planet base;
    DummyScript *script = base.adopt(new DummyScript);
    script->file(Value("test/fixtures/script_test_new_file.txt"));
    Value res;
    script->inspect(&res);
    assert_equal("script:\"\" file:\"test/fixtures/script_test_new_file.txt\" reload:1", res.lazy_json());
  }
};
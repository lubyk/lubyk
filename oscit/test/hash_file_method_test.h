#include "test_helper.h"
#include "oscit/root.h"
#include "oscit/hash_file_method.h"

#include <sstream>
#include <fstream>    // file io

#define HASH_FILE_METHOD_PATH "simple_view.json"

class HashFileMethodTest : public TestHelper
{
public:
  HashFileMethodTest() {
    preserve(fixture_path(HASH_FILE_METHOD_PATH));
  }

  void tearDown() {
    restore(fixture_path(HASH_FILE_METHOD_PATH));
  }

  void test_read( void ) {
    Root root(false);
    root.adopt(new HashFileMethod("simple_view", fixture_path(HASH_FILE_METHOD_PATH), std::string("Basic synth view.")));
    Value res = root.call("/simple_view");
    assert_true(res.is_string())
    assert_equal("{\n  \"x\":0, \"y\":0, \"width\":500, \"height\":", res.str().substr(0, 40));
  }

  void test_write( void ) {
    Root root(false);
    root.adopt(new FileMethod("simple_view", fixture_path(HASH_FILE_METHOD_PATH), std::string("Basic synth view.")));
    Value res = root.call("/simple_view", Value("Yoba"));
    assert_true(res.is_string());
    assert_equal("Yoba", res.str());

    res = root.call("/simple_view");
    assert_true(res.is_string());
    assert_equal("Yoba", res.str());

    std::ifstream in(fixture_path(HASH_FILE_METHOD_PATH).c_str(), std::ios::in);
      std::ostringstream oss;
      oss << in.rdbuf();
    in.close();
    assert_equal("Yoba", oss.str());
  }

  void test_update( void ) {
    assert_true(false); // TODO
  }
};


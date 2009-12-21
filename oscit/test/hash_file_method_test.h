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
    assert_true(res.is_hash())
    assert_equal("{\"x\":0, \"y\":0, \"width\":500, \"height\":200", res.to_json().substr(0, 40));
  }

  void test_write_hash( void ) {
    Root root(false);
    root.adopt(new HashFileMethod("simple_view", fixture_path(HASH_FILE_METHOD_PATH), std::string("Basic synth view.")));
    Value res = root.call("/simple_view", Value(Json("{\"one\":1}")));
    assert_true(res.is_hash());
    assert_equal("{\"one\":1}", res.to_json());

    res = root.call("/simple_view");
    assert_true(res.is_hash());
    assert_equal("{\"one\":1}", res.to_json());

    std::ifstream in(fixture_path(HASH_FILE_METHOD_PATH).c_str(), std::ios::in);
      std::ostringstream oss;
      oss << in.rdbuf();
    in.close();
    assert_equal("{\"one\":1}", oss.str());
  }

  void test_write_bad_format( void ) {
    Root root(false);
    HashFileMethod *fm = root.adopt(new HashFileMethod("simple_view", fixture_path(HASH_FILE_METHOD_PATH), std::string("Basic synth view.")));
    assert_false(fm->can_receive(Value(Json("Yoba"))));
    Value res = root.call("/simple_view", Value("Yoba"));
    assert_true(res.is_error());
    assert_equal(BAD_REQUEST_ERROR, res.error_code());

    // content not changed
    res = root.call("/simple_view");
    assert_true(res.is_hash());
    assert_equal("{\"x\":0, \"y\":0, \"width\":500, \"height\":200", res.to_json().substr(0, 40));

    std::ifstream in(fixture_path(HASH_FILE_METHOD_PATH).c_str(), std::ios::in);
      std::ostringstream oss;
      oss << in.rdbuf();
    in.close();
    assert_equal("{\n  \"x\":0, \"y\":0, \"width\":500, \"height\":", oss.str().substr(0, 40));
  }

  void test_update( void ) {
    Root root(false);
    root.adopt(new HashFileMethod("simple_view", fixture_path(HASH_FILE_METHOD_PATH), std::string("Basic synth view.")));
    Value res = root.call("/simple_view/update", Value(Json("{parts:{1:{x:11}}}")));
    assert_true(res.is_hash());
    assert_equal("{\"parts\":{\"1\":{\"x\":11}}}", res.to_json());

    res = root.call("/simple_view");
    assert_true(res.is_hash());
    assert_equal("\"parts\":{\"1\":{\"class\":\"Slider\", \"x\":11, \"y\":10, \"width\":30", res.to_json().substr(42, 58));
    assert_equal(11, res["parts"]["1"]["x"].r);

    std::ifstream in(fixture_path(HASH_FILE_METHOD_PATH).c_str(), std::ios::in);
      std::ostringstream oss;
      oss << in.rdbuf();
    in.close();
    assert_equal("\"parts\":{\"1\":{\"class\":\"Slider\", \"x\":11, \"y\":10, \"width\":30", oss.str().substr(42, 58));
  }
};


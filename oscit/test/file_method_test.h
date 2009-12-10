#include "test_helper.h"
#include "oscit/root.h"
#include "oscit/file_method.h"

#include <sstream>
#include <fstream>    // file io

class FileMethodTest : public TestHelper
{
public:
  FileMethodTest() {
    preserve(fixture_path("simple_view.xml"));
  }

  void tearDown() {
    restore(fixture_path("simple_view.xml"));
  }

  void test_read( void ) {
    Root root(false);
    root.adopt(new FileMethod("simple_view", fixture_path("simple_view.xml"), std::string("Basic synth view.")));
    Value res = root.call("/simple_view");
    assert_true(res.is_string())
    assert_equal("<view x=\'100\' y=\'100\' width=\'200\' height", res.str().substr(0, 40));
  }

  void test_write( void ) {
    Root root(false);
    root.adopt(new FileMethod("simple_view", fixture_path("simple_view.xml"), std::string("Basic synth view.")));
    Value res = root.call("/simple_view", Value("Yoba"));
    assert_true(res.is_string());
    assert_equal("Yoba", res.str());

    res = root.call("/simple_view");
    assert_true(res.is_string());
    assert_equal("Yoba", res.str());

    std::ifstream in(fixture_path("simple_view.xml").c_str(), std::ios::in);
      std::ostringstream oss;
      oss << in.rdbuf();
    in.close();
    assert_equal("Yoba", oss.str());
  }
};


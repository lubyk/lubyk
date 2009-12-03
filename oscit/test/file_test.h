#include "test_helper.h"
#include "oscit/file.h"

class FileTest : public TestHelper
{
public:
  FileTest() {
    preserve(fixture_path("simple_view.xml"));
  }

  void tearDown() {
    restore(fixture_path("simple_view.xml"));
  }

  void test_read( void ) {
    File file(fixture_path("simple_view.xml"));
    Value content = file.read();
    assert_true(content.is_string())
    assert_equal("<view x=\'100\' y=\'100\' width=\'200\' height", content.str().substr(0, 40));
  }
  
  void test_write( void ) {
    File file1(fixture_path("simple_view.xml"));
    File file2(fixture_path("simple_view.xml"));
    assert_true(file1.write("Yoba"));
    Value content = file2.read();
    assert_true(content.is_string())
    assert_equal("Yoba", content.str());
  }

  void test_append( void ) {
    File file(fixture_path("simple_view.xml"));
    file.write("");
    file.append("Hello");
    file.append(" World!");
    assert_equal("Hello World!", file.read().str());
  }

  void test_do_not_keep_handle( void ) {
    File file(fixture_path("simple_view.xml"));
    File file2(fixture_path("simple_view.xml"));
    std::string new_content = "dummy new content";
    file2.write(new_content);
    assert_equal(new_content, file.read().str());
  }
};


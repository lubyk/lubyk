#include "test_helper.h"
#include "oscit/file.h"

#define FILE_TEST_PATH "simple_view.json"
class FileTest : public TestHelper
{
public:
  FileTest() {
    preserve(fixture_path(FILE_TEST_PATH));
  }

  void tearDown() {
    restore(fixture_path(FILE_TEST_PATH));
  }

  void test_read( void ) {
    File file(fixture_path(FILE_TEST_PATH));
    Value content = file.read();
    assert_true(content.is_string())
    assert_equal("{\n  \"x\":0, \"y\":0, \"width\":500, \"height\":", content.str().substr(0, 40));
  }

  void test_write( void ) {
    File file1(fixture_path(FILE_TEST_PATH));
    File file2(fixture_path(FILE_TEST_PATH));
    assert_true(file1.write("Yoba"));
    Value content = file2.read();
    assert_true(content.is_string())
    assert_equal("Yoba", content.str());
  }

  void test_append( void ) {
    File file(fixture_path(FILE_TEST_PATH));
    file.write("");
    file.append("Hello");
    file.append(" World!");
    assert_equal("Hello World!", file.read().str());
  }

  void test_do_not_keep_handle( void ) {
    File file(fixture_path(FILE_TEST_PATH));
    File file2(fixture_path(FILE_TEST_PATH));
    std::string new_content = "dummy new content";
    file2.write(new_content);
    assert_equal(new_content, file.read().str());
  }
};


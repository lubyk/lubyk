#include "test_helper.h"
#include "oscit/file.h"

class FileTest : public TestHelper
{
public:
  FileTest() : saved_content_(10) {
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

private:
  void preserve(const char *path) {
    preserve(std::string(path));
  }

  void preserve(const std::string &path) {
    if (!saved_content_.has_key(path)) {
      File file(path);
      Value content = file.read();
      if (content.is_string())
        saved_content_.set(path, content.str());
    }
  }

  void restore(const char *path) {
    restore(std::string(path));
  }

  void restore(const std::string &path) {
    std::string content;
    if (saved_content_.get(path, &content)) {
      File file(path);
      file.write(content);
    }
  }
  THash<std::string, std::string> saved_content_;
};


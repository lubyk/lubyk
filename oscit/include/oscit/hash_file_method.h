#ifndef OSCIT_INCLUDE_OSCIT_HASH_FILE_METHOD_H_
#define OSCIT_INCLUDE_OSCIT_HASH_FILE_METHOD_H_
#include <string>

#include "oscit/file_method.h"

namespace oscit {

/** This class is responsible for managaging access to the filesystem. All operations
 * are atomic and the File never holds a lock on the file representation. This means
 * that you should prepare all the content in a string instead of calling File#append
 * again and again.
 */
class HashFileMethod : public Object {
public:
  TYPED("Object.HashFileMethod")

  HashFileMethod(const char *name, const char *path)
      : Object(name, HashIO("Read/write hash file content.")),
        file_(path) {
    create_methods();
  }

  HashFileMethod(const std::string &name, const std::string &path)
      : Object(name, HashIO("Read/write hash file content.")),
        file_(path) {
    create_methods();
  }

  HashFileMethod(const char *name, const char *path, const char *info)
      : Object(name, HashIO(info)),
        file_(path) {
    create_methods();
  }

  HashFileMethod(const std::string &name, const std::string &path, const std::string &info)
      : Object(name, HashIO(info)),
        file_(path) {
    create_methods();
  }

  virtual ~HashFileMethod() {}

  /** Read/write the hash content as string.
   * The file path was provided on the object's creation.
   */
  virtual const Value trigger(const Value &hash);

  /** Update the content of the file's hash by doing a deep merge.
   * For example if the current content is {"one":{"x":45, "y":100}, "two":{...}} and we
   * receive an update with {"one":{"x":40}, "two":null}, the final result will be
   * {"one":{"x":40, "y":100}}.
   */
  const Value update(const Value &hash);

  ///** Set root and create a 'xxx/update' method that is connected to "update"
  // */
  //virtual void set_root(Root *root);

private:
  File file_;

  /** Creates a 'xxx/update' method that is connected to "update"
   */
  void create_methods();

  /** Current file content.
   */
  Value hash_;
};
} // oscit

#endif // OSCIT_INCLUDE_OSCIT_HASH_FILE_METHOD_H_

#ifndef OSCIT_INCLUDE_OSCIT_FILE_METHOD_H_
#define OSCIT_INCLUDE_OSCIT_FILE_METHOD_H_
#include "oscit/root.h"
#include "oscit/file.h"

namespace oscit {

class FileMethod : public Object
{
public:
  /** Class signature. */
  TYPED("Object.FileMethod")

  FileMethod(const char *name, const char *path)
      : Object(name, StringIO("data", "Read/write file content.")),
        file_(path) {}

  FileMethod(const std::string &name, const std::string &path)
      : Object(name, StringIO("data", "Read/write file content.")),
        file_(path) {}

  FileMethod(const char *name, const char *path, const char *info)
      : Object(name, StringIO("data", info)),
        file_(path) {}

  FileMethod(const std::string &name, const std::string &path, const std::string &info)
      : Object(name, StringIO("data", info)),
        file_(path) {}

  /** Read/write to file. The file path was provided on the object's creation.
    */
  virtual const Value trigger(const Value &val) {
    if (!val.is_string()) return file_.read();

    if (!file_.write(val.str())) {
      // could not write to file
      return file_.last_error();
    }

    return val;
  }
  
private:
  File file_;
};

} // oscit

#endif // OSCIT_INCLUDE_OSCIT_FILE_METHOD_H_

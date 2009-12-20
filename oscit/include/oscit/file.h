#ifndef OSCIT_INCLUDE_OSCIT_FILE_H_
#define OSCIT_INCLUDE_OSCIT_FILE_H_
#include <string>

#include "oscit/values.h"

namespace oscit {

/** This class is responsible for managaging access to the filesystem. All operations
 * are atomic and the File never holds a lock on the file representation. This means
 * that you should prepare all the content in a string instead of calling File#append
 * again and again.
 */
class File {
public:
  File(const char *path) : path_(path) {}
  File(const std::string &path) : path_(path) {}
  virtual ~File() {}

  /** Read all content of file into a Value.
   * @return a StringValue on success and an ErrorValue in case of failure.
   */
  Value read();

  /** Replace the current file's content by the new content.
   * Returns false on failure (use File#last_error() for an ErrorValue).
   */
  bool write(const std::string& data);

  /** Replace the current file's content by the new content.
   * Returns false on failure (use File#last_error() for an ErrorValue).
   */
  bool write(const char *data) { return write(std::string(data)); }

  /** Append some content to the end of the file.
   * Returns false on failure (use File#last_error() for an ErrorValue).
   */
  bool append(const std::string& data);

  /** Append some content to the end of the file.
   * Returns false on failure (use File#last_error() for an ErrorValue).
   */
  bool append(const char *data) { return append(std::string(data)); }

  /** Return true if an error occured (during initialization, read, write, etc).
   */
  bool has_error() {
    return last_error_.is_error();
  }

  /** If an error occured, use this method to get information on the error.
   */
  Value last_error() const {
    return last_error_;
  }

  std::string path() const {
    return path_;
  }

private:
  Value last_error_;
  std::string path_;
};
} // oscit

#endif // OSCIT_INCLUDE_OSCIT_FILE_H_
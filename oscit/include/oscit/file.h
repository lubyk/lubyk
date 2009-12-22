/*
  ==============================================================================

   This file is part of the OSCIT library (http://rubyk.org/liboscit)
   Copyright (c) 2007-2009 by Gaspard Bucher - Buma (http://teti.ch).

  ------------------------------------------------------------------------------

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

  ==============================================================================
*/

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
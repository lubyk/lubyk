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

#ifndef OSCIT_INCLUDE_OSCIT_ERROR_H_
#define OSCIT_INCLUDE_OSCIT_ERROR_H_
#include "oscit/values.h"
#include "oscit/reference_counted.h"

namespace oscit {

/** Unique identifier for osc type tags strings.
 *  Redefinition from value.h because of inclusion order.
 */
typedef uint TypeTagID;

enum ErrorCode {
  BAD_REQUEST_ERROR = 400,
  NOT_FOUND_ERROR   = 404,
  INTERNAL_SERVER_ERROR = 500,
  UNKNOWN_ERROR,
};

/** A Error stores an error description with an error code. */
class Error : public ReferenceCounted
{
 public:
  explicit Error() : code_(UNKNOWN_ERROR), message_("unknown error") {}
  explicit Error(ErrorCode code, const char *message) : code_(code), message_(message) {}

  /** Copy constructor (needed because Value relies on it in its own copy constructor. */
  Error(const Error& other) : code_(other.code_), message_(other.message_) {}

  const char * type_tag() const { return "s"; }

  TypeTagID type_id() const { return H("s"); }

  ErrorCode code() const { return code_; }

  void set_code(ErrorCode code) { code_ = code; }

  const char * str() const { return message_.c_str(); }

  const std::string& message() const { return message_; }

  void set_message(const char *string) { message_ = string; }

  Error& append(const char *str) {
    message_.append(str);
    return *this;
  }

  void set_message(const std::string &string) { message_ = string; }

  bool operator==(const Error& other) const {
    return code_ == other.code_ && message_ == other.message_;
  }

 private:

  ErrorCode   code_;
  std::string message_;
};

} // oscit

#endif // OSCIT_INCLUDE_OSCIT_ERROR_H_

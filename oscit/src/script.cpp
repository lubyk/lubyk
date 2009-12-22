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

#include "oscit/script.h"

#include <sys/types.h>
#include <sys/stat.h> // stat for file modification time

#include <fstream>    // file io
#include <string>
#include <sstream>

#include "oscit/values.h"

namespace oscit {

#define ONE_SECOND 1000.0

Script::Script() : script_ok_(false), script_mod_time_(0), reload_every_(1), next_reload_(0), current_time_(0) {}

Script::~Script() {}

const Value Script::script(const Value &val) {
  if (!val.is_nil()) {
    script_       = val.str();
    Value res = save_script();
    if (res.is_error()) {
      set_script_ok(false);
      return res;
    }

    res = eval_script();
    if (res.is_error()) {
      set_script_ok(false);
      return res;
    }

    set_script_ok(true);
  }
  return Value(script_);
}

const Value Script::file(const Value &val) {
  if (!val.is_nil()) {
    script_file_  = val.str();
    return load_script_from_file(true);
  }
  return Value(script_file_);
}

const Value Script::reload(const Value &val) {
  if (!val.is_nil()) {
    reload_every_ = val.r > 0 ? val.r : 0;
    set_next_reload();
  }
  return Value(reload_every_);
}

void Script::inspect(Value *hash) {
  hash->set("script", script_);
  hash->set("file", script_file_);
  hash->set("reload", reload_every_);
}

void Script::reload_script(time_t current_time) {
  current_time_ = current_time;
  if ( !next_reload_ || (next_reload_ > current_time_) ) {
    return;
  }
  set_next_reload();
  load_script_from_file(false);
}

void Script::set_script_ok(bool state) {
  script_ok_ = state;
}

const Value Script::load_script_from_file(bool is_new) {
  struct stat info;

  if (stat(script_file_.c_str(), &info)) {
    set_script_ok(false);
    script_mod_time_ = 0;
    set_next_reload();
    return Value(BAD_REQUEST_ERROR, std::string("Could not stat '").append(script_file_).append("'."));
  }

  if (!is_new && info.st_mtime == script_mod_time_) {
    // file did not change, skip
    return Value(script_file_);
  }

  script_mod_time_ = info.st_mtime;

  // TODO: refactor using File *script_file_.... ///
  std::ifstream in(script_file_.c_str(), std::ios::in);
    std::ostringstream oss;
    oss << in.rdbuf();
  in.close();

  script_ = oss.str();

  Value res = eval_script();
  set_script_ok(!res.is_error());
  if (res.is_error()) return res;

  return Value(script_file_);
}

const Value Script::save_script() {
  if (script_file_ == "") return gNilValue;

  // TODO: check file is writeable, limit path, etc
  //   return Value(INTERNAL_SERVER_ERROR, std::string("Could not save to '").append(script_file_).append("' (.....)."));

  // try .. ?
  std::ofstream out(script_file_.c_str(), std::ios::out);
    out << script_;
  out.close();

  struct stat info;

  if (stat(script_file_.c_str(), &info)) {
    set_script_ok(false);
    return Value(BAD_REQUEST_ERROR, std::string("Could not stat '").append(script_file_).append("'."));
  }

  script_mod_time_ = info.st_mtime;
  return gNilValue;
}

void Script::set_next_reload() {
  if (script_file_ == "") {
    next_reload_ = 0;
  } else {
    next_reload_ = current_time_ + (reload_every_ * ONE_SECOND);
  }
}

}  // oscit
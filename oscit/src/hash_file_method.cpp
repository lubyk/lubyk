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

#include "oscit/hash_file_method.h"

#include "oscit/method.h"

namespace oscit {


void HashFileMethod::create_methods() {
  adopt(new TMethod<HashFileMethod, &HashFileMethod::update>(this, "update", HashIO("Hash to deep merge in current content.")));
}

const Value HashFileMethod::trigger(const Value &val) {
  Value h;
  if (!val.is_hash()) {
    if (hash_.is_empty()) {
      Value str(file_.read());
      h.set((Json)str.str());
      if (!h.is_hash()) {
        std::cerr << url() << ": error, hash file content '" << file_.path() << "' is not a Hash !\n";
      } else {
        hash_ = h;
      }
    }
    return hash_;
  }

  if (!file_.write(val.to_json())) {
    // could not write to file
    return file_.last_error();
  } else {
    hash_ = val;
  }

  return hash_;
}

const Value HashFileMethod::update(const Value &val) {
  if (!val.is_hash()) return gNilValue;

  if (hash_.is_empty()) {
    // force file loading
    Value res = trigger(gNilValue);
    if (res.is_error()) return res;
  }

  hash_.deep_merge(val);
  if (!file_.write(hash_.to_json())) {
    // could not write to file
    return file_.last_error();
  } else {
    return val;
  }
}


} // oscit

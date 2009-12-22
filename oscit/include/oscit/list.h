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

#ifndef OSCIT_INCLUDE_OSCIT_LIST_H_
#define OSCIT_INCLUDE_OSCIT_LIST_H_
#include "oscit/value_types.h"
#include "oscit/reference_counted.h"

#include <vector>

namespace oscit {

class Value;

/** A List stores an array of Values. */
class List : public ReferenceCounted
{
 public:
  explicit List() {
    init_with_type_tag("");
  }

  explicit List(const char *type_tag) {
    init_with_type_tag(type_tag);
  }

  explicit List(TypeTag type_tag) {
    init_with_type_tag(type_tag.str_);
  }

  // deep copy
  List(const List &list);

  ~List() {
    clear();
  }

  const char * type_tag() const {
    return type_tag_;
  }

  TypeTagID type_id() const {
    return type_id_;
  }

  const Value *operator[](size_t pos) const {
    return values_[pos];
  }

  Value *operator[](size_t pos) {
    return values_[pos];
  }

  Value *last() {
    return values_.size() > 0 ? values_[values_.size() - 1] : NULL;
  }

  Value *first() {
    return values_.size() > 0 ? values_[0] : NULL;
  }
  /** Replace a value at a given position, checking for range and making sure
   *  the type_tag of the list remains in sync. */
  void set_value_at(size_t pos, const Value &val);

  size_t size() const {
    return values_.size();
  }

  void push_back(const Value &val);

  void push_front(const Value &val);

  const char *set_type_tag(const char *type_tag) {
    clear();
    return init_with_type_tag(type_tag);
  }

  bool operator==(const List &other) const;

 private:
  void clear();

  void update_type_tag() {
    type_tag_ = type_tag_storage_.c_str();
    type_id_ = H(type_tag_); // FIXME: length limit is ~12 ...
  }

  const char *init_with_type_tag(const char *type_tag);

  std::string type_tag_storage_;
  const char  *type_tag_;
  TypeTagID   type_id_;
  std::vector<Value*> values_;
};

} // oscit

#endif // OSCIT_INCLUDE_OSCIT_LIST_H_

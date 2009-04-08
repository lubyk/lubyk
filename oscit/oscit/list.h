#ifndef _OSCIT_LIST_H_
#define _OSCIT_LIST_H_
#include "oscit/values.h"
#include <vector>

namespace oscit {

/** A List stores an array of Values. */
class List
{
 public:
  explicit List() {
    init("");
  }
  
  explicit List(const char *type_tag) {
    init(type_tag);
  }
  
  explicit List(TypeTag type_tag) {
    init(type_tag.str_);
  }
  
  /** Copy constructor (needed because Value relies on it in its own copy constructor. */
  List(const List& list) : type_tag_storage_(list.type_tag_storage_), values_(list.values_) {
    type_tag_ = type_tag_storage_.c_str();
    type_tag_id_ = hashId(type_tag_);
  }
  
  const char * type_tag() const {
    return type_tag_;
  }
  
  TypeTagID type_tag_id() const {
    return type_tag_id_;
  }
  
  const Value& operator[](size_t pos) const {
    return values_[pos];
  }
  
  Value& operator[](size_t pos) {
    return values_[pos];
  }
  
  /** Replace a value at a given position, checking for range and making sure
   *  the type_tag of the list remains in sync. */
  void set_value_at(size_t pos, const Value &val) {
    if (pos >= size()) return;
    if (val.is_list()) return; // NOT SUPPORTED YET (probably never needed)
    values_[pos] = val;
    type_tag_storage_.replace(pos,1,val.type_tag());
    type_tag_ = type_tag_storage_.c_str();
    type_tag_id_ = hashId(type_tag_);
  }
  
  size_t size() const {
    return values_.size();
  }
  
  void push_back(const Value &val) {
    if (val.is_list()) {
      size_t sz = val.size();
      for(size_t i = 0; i < sz; ++i) {
        push_back(val[i]);
      }
    } else {
      values_.push_back(val);
      type_tag_storage_.append(val.type_tag());
      type_tag_ = type_tag_storage_.c_str();
      type_tag_id_ = hashId(type_tag_);
    }
  }
  
  void push_front(const Value &val) {
    if (val.is_list()) {
      size_t sz = val.size();
      values_.insert(values_.begin(), sz, gNilValue);
      for(size_t i = 0; i < sz; ++i) {
        values_[i] = val[i];
      }
    } else {
      values_.insert(values_.begin(), 1, val);
    }
    type_tag_storage_.insert(0, val.type_tag());
    type_tag_ = type_tag_storage_.c_str();
    type_tag_id_ = hashId(type_tag_);
  }
 private:
  void init(const char *type_tag) {
    type_tag_storage_ = type_tag;
    type_tag_    = type_tag_storage_.c_str();
    type_tag_id_ = hashId(type_tag_);

    const char * c = type_tag_;

    while ( *c ) {
      values_.push_back(Value(*c));
      c++;
    }
  }
  
  std::string type_tag_storage_;
  const char  *type_tag_;
  TypeTagID   type_tag_id_;
  std::vector<Value> values_;
};

} // oscit
#endif // _OSCIT_LIST_H_
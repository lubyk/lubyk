#ifndef _OSCIT_LIST_H_
#define _OSCIT_LIST_H_
#include "oscit/values.h"
#include <vector>
#include "oscit/reference_counted.h"

namespace oscit {

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
    values_.push_back(val);
    if (val.is_list()) {
      type_tag_storage_.append("[").append(val.type_tag()).append("]");
    } else {
      type_tag_storage_.append(val.type_tag());
    }
    type_tag_ = type_tag_storage_.c_str();
    type_tag_id_ = hashId(type_tag_);
  }
  
  void push_front(const Value &val) {
    values_.insert(values_.begin(), 1, val);
    if (val.is_list()) {
      type_tag_storage_ = std::string("[").append(val.type_tag()).append("]").append(type_tag_storage_);
    } else {
      type_tag_storage_.insert(0, val.type_tag());
    }
    type_tag_ = type_tag_storage_.c_str();
    type_tag_id_ = hashId(type_tag_);
  }
  
  const char *set_type_tag(const char *type_tag) {
    values_.clear();
    return init_with_type_tag(type_tag);
  }
  
 private:
  const char *init_with_type_tag(const char *type_tag) { 
   type_tag_storage_ = type_tag;
   type_tag_    = type_tag_storage_.c_str();
   type_tag_id_ = hashId(type_tag_);
   Value tmp;

   const char * c = type_tag;

   while ( *c ) {
     if (*c == '[') {
       c = tmp.set_type_tag(c+1);
       values_.push_back(tmp);
     } else if (*c == ']') {
       // finished building here, exit
       type_tag_storage_ = type_tag_storage_.substr(0, c - type_tag);
       type_tag_    = type_tag_storage_.c_str();
       type_tag_id_ = hashId(type_tag_);
       return c+1;
     } else {
       values_.push_back(Value(*c));
     }
     c++;
   }
   return c;
  }
  
  std::string type_tag_storage_;
  const char  *type_tag_;
  TypeTagID   type_tag_id_;
  std::vector<Value> values_;
};

} // oscit
#endif // _OSCIT_LIST_H_
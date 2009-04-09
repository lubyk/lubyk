#ifndef _OSCIT_LIST_H_
#define _OSCIT_LIST_H_
#include "oscit/values.h"
#include "oscit/tsmart_ptr.h"
#include <vector>

namespace oscit {

class List;
  
class ListData
{
 private:
  friend class List;
  
  ListData(const char *type_tag) : type_tag_storage_(type_tag) {
    type_tag_    = type_tag_storage_.c_str();
    type_tag_id_ = hashId(type_tag_);

    const char * c = type_tag_;

    while ( *c ) {
      values_.push_back(Value(*c));
      ++c;
    }
  }

  size_t size() const { return values_.size(); }

  void set_value_at(size_t pos, const Value &val) {
    if (pos >= size()) return;
    if (val.is_list()) return; // NOT SUPPORTED YET (probably never needed)
    values_[pos] = val;
    type_tag_storage_.replace(pos,1,val.type_tag());
    type_tag_ = type_tag_storage_.c_str();
    type_tag_id_ = hashId(type_tag_);
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
  
  std::string type_tag_storage_;
  const char  *type_tag_;
  TypeTagID   type_tag_id_;
  std::vector<Value> values_;
};

/** A List stores an array of Values. */
class List : public TSmartPtr<ListData>
{
 public:
  explicit List() : TSmartPtr<ListData>(new ListData("")) {}
  
  explicit List(const char *type_tag) : TSmartPtr<ListData>(new ListData(type_tag))  {}
  
  explicit List(TypeTag type_tag) : TSmartPtr<ListData>(new ListData(type_tag.string_))  {}
  
  const char *type_tag() const { return ptr_->data_->type_tag_; }
  
  TypeTagID type_tag_id() const { return ptr_->data_->type_tag_id_; }
  
  const Value &operator[](size_t pos) const { return ptr_->data_->values_[pos];}
  
  Value &operator[](size_t pos) { return ptr_->data_->values_[pos]; }
  
  /** Replace a value at a given position, checking for range and making sure
   *  the type_tag of the list remains in sync. */
  void set_value_at(size_t pos, const Value &val) { ptr_->data_->set_value_at(pos, val); }
  
  size_t size() const { return ptr_->data_->size(); }
  
  void push_back(const Value &val) { ptr_->data_->push_back(val); }
  
  void push_front(const Value &val) { ptr_->data_->push_front(val); }
};

} // oscit
#endif // _OSCIT_LIST_H_
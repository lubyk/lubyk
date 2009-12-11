#include "oscit/values.h"

#include <vector>

#include "oscit/reference_counted.h"

namespace oscit {

List::List(const List& list) {
  std::vector<Value*>::const_iterator it,end;
  end = list.values_.end();
  for (it = list.values_.begin(); it != end; ++it) {
    values_.push_back(new Value(**it));
  }
  type_tag_storage_ = list.type_tag_storage_;
  update_type_tag();
}

/** Replace a value at a given position, checking for range and making sure
 *  the type_tag of the list remains in sync. */
void List::set_value_at(size_t pos, const Value &val) {
  if (pos >= size()) return;
  if (val.is_list()) {
    if (values_[pos]->type_id() != val.type_id()) return; // TODO: NOT SUPPORTED YET
    values_[pos]->set(val);
  } else {
    values_[pos]->set(val);
    // FIXME: what if we had a list: "ff[sfs]ss", replace elem at 2 --> "ffsss"
    type_tag_storage_.replace(pos,1,val.type_tag());
    update_type_tag();
  }
}

void List::push_back(const Value &val) {
  values_.push_back(new Value(val));
  if (val.is_list()) {
    type_tag_storage_.append("[").append(val.type_tag()).append("]");
  } else {
    type_tag_storage_.append(val.type_tag());
  }
  update_type_tag();
}

void List::push_front(const Value &val) {
  values_.insert(values_.begin(), 1, new Value(val));
  if (val.is_list()) {
    type_tag_storage_ = std::string("[").append(val.type_tag()).append("]").append(type_tag_storage_);
  } else {
    type_tag_storage_.insert(0, val.type_tag());
  }
  update_type_tag();
}

 void List::clear() {
   std::vector<Value*>::iterator it,end;
   end = values_.end();
   for (it = values_.begin(); it != end; ++it) {
     delete *it;
   }
   values_.clear();
 }

const char *List::init_with_type_tag(const char *type_tag) {
 type_tag_storage_ = type_tag;
 update_type_tag();

 const char * c = type_tag;

 while ( *c ) {
   if (*c == '[') {
     Value *tmp = new Value;
     c = tmp->set_type_tag(c+1);
     values_.push_back(tmp);
   } else if (*c == ']') {
     // finished building here, exit
     type_tag_storage_ = type_tag_storage_.substr(0, c - type_tag);
     update_type_tag();
     return c+1;
   } else {
     values_.push_back(new Value(*c));
     c++;
   }
 }
 return c;
}

bool List::operator==(const List &other) const {
  if (type_id_          != other.type_id_ ||
      values_.size()    != other.values_.size() ||
      type_tag_storage_ != other.type_tag_storage_) return false;

  int sz = values_.size();
  for (int i = 0; i < sz; ++i) {
    if (*values_[i] != *other.values_[i]) return false;
  }

  return true;
}
} // oscit

#include "oscit/root.h"
#include "oscit/alias.h"

#include "src/globals.cpp"

namespace oscit {
  
BaseObject::~BaseObject() {
  std::list<Alias*>::iterator it, end = aliases_.end();
  // notify parent and root
  if (parent_) parent_->release(this);
  
  for (it = aliases_.begin(); it != end; ++it) {
    // to avoid notification to this dying object
    (*it)->unlink_original();
    delete *it;
  }
  
  clear();
}

const Value BaseObject::set(const Value &val) {
  HashIterator it;
  HashIterator end = val.end();
  Value param;
  Value res;
  BaseObject * obj;
  
  if (context_) context_->lock();
    for (it = val.begin(); it != end; ++it) {
      if ((obj = child(*it)) && val.get(*it, &param)) {
        res.set(*it, root_->call(obj, param));
      } else {
        res.set(*it, ErrorValue(NOT_FOUND_ERROR, *it));
      }
    }
  if (context_) context_->unlock();
  
  return res;
}

bool BaseObject::set_all_ok(const Value &val) {
  HashIterator it;
  HashIterator end = val.end();
  Value param;
  bool all_ok = true;
  BaseObject * obj;
  
  for (it = val.begin(); it != end; ++it) {
    if ((obj = child(*it)) && val.get(*it, &param)) {
      all_ok = !root_->call(obj, param).is_error() && all_ok;
    } else {
      all_ok = false;
    }
  }  
  return all_ok;
}

/** Inform the object of an alias that depends on it. */
void BaseObject::register_alias(Alias *alias) {
  aliases_.push_back(alias);
}

/** Inform the object that an alias no longer exists. */
void BaseObject::unregister_alias(Alias *alias) {
  aliases_.remove(alias);
}

/** Free the child from the list of children. */
void BaseObject::release(BaseObject *object) {
  children_.remove_element(object);
  if (root_) root_->unregister_object(object);
}

void BaseObject::moved()
{ 
  // 1. get new name from parent, register as child
  if (parent_) parent_->register_child(this);
  
  register_url();
}

void BaseObject::register_child(BaseObject *object) {
  // 1. reset hash
  children_.remove_element(object);
  
  // 2. get valid name
  while (child(object->name_)) {
    object->find_next_name();
  }
  
  // 3. set hash back
  children_.set(object->name_, object);
}

void BaseObject::register_url() {
  BaseObject * obj;
  string_iterator it;
  string_iterator end = children_.end();
  
  // 1. rebuild url
  if (parent_) {
    // build fullpath
    url_ = std::string(parent_->url()).append("/").append(name_);
    if (parent_->root_) parent_->root_->register_object(this);
  } else {
    // no parent
    url_ = name_;
    if (root_) root_->unregister_object(this);
  }
  
  // 3. update children
  for(it = children_.begin(); it != end; it++) {
    if (children_.get(*it, &obj)) obj->register_url();
  }
}

void BaseObject::clear() {
  string_iterator it;
  string_iterator end = children_.end();

  // destroy all children
  for(it = children_.begin(); it != end; it++) {
    BaseObject * child;
    if (children_.get(*it, &child)) {
      // to avoid 'release' call (would alter children_)
      child->parent_ = NULL;
      if (root_) root_->unregister_object(child);
      delete child;
    }
  }
  children_.clear();
}

} // namespace oscit
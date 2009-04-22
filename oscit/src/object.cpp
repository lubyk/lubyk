#include "oscit/root.h"
#include "oscit/alias.h"

pthread_key_t oscit::Thread::sThisKey;


namespace oscit {
  
Object::~Object() {
  std::list<Alias*>::iterator it, end = aliases_.end();
  // notify parent and root
  set_parent(NULL);
  set_root(NULL);
  
  for (it = aliases_.begin(); it != end; ++it) {
    // to avoid notification to this dying object
    (*it)->unlink_original();
    delete *it;
  }
  
  clear();
}

const Value Object::set(const Value &val) {
  HashIterator it;
  HashIterator end = val.end();
  Value param;
  Value res;
  Object * obj;
  
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

bool Object::set_all_ok(const Value &val) {
  HashIterator it;
  HashIterator end = val.end();
  Value param;
  bool all_ok = true;
  Object * obj;
  
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
void Object::register_alias(Alias *alias) {
  aliases_.push_back(alias);
}

/** Inform the object that an alias no longer exists. */
void Object::unregister_alias(Alias *alias) {
  aliases_.remove(alias);
}

/** Free the child from the list of children. */
void Object::unregister_child(Object *object) {
  children_.remove_element(object);
}

void Object::moved() { 
  // 1. get new name from parent, register as child
  if (parent_) {
    // rebuild fullpath
    url_ = std::string(parent_->url()).append("/").append(name_);
    set_root(parent_->root_);
    set_context(parent_->context_);
  } else {
    // no parent
    url_ = name_;
    set_root(NULL);
  }
  
  string_iterator it;
  string_iterator end = children_.end();
  Object *child;
  
  // 3. update children
  for(it = children_.begin(); it != end; it++) {
    if (children_.get(*it, &child)) child->moved();
  }
}

void Object::register_child(Object *object) {
  // 1. make sure it is not in dictionary
  unregister_child(object);
  
  // 2. get valid name
  while (child(object->name_)) {
    object->find_next_name();
  }
  
  // 3. add to dictionary with new name
  children_.set(object->name_, object);
}

void Object::set_root(Root *root) {
  if (root_) root_->unregister_object(this);
  root_ = root;
  if (root_) root_->register_object(this);
}

void Object::set_parent(Object *parent) {
  if (parent_) parent_->unregister_child(this);
  parent_ = parent;
  if (parent_) parent_->register_child(this);
  moved();
}

void Object::clear() {
  string_iterator it;
  string_iterator end = children_.end();

  // destroy all children
  for(it = children_.begin(); it != end; it++) {
    Object * child;
    if (children_.get(*it, &child)) {
      // to avoid 'unregister_child' call (would alter children_)
      child->parent_ = NULL;
      if (root_) root_->unregister_object(child);
      delete child;
    }
  }
  children_.clear();
}

} // namespace oscit
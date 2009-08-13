#include "oscit/root.h"
#include "oscit/alias.h"

pthread_key_t oscit::Thread::sThisKey = NULL;


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

  for (it = val.begin(); it != end; ++it) {
    if ((obj = child(*it)) && val.get(*it, &param)) {
      res.set(*it, root_->call(obj, param, context_));
    } else {
      res.set(*it, ErrorValue(NOT_FOUND_ERROR, *it));
    }
  }

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
      all_ok = !root_->call(obj, param, context_).is_error() && all_ok;
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
  } else if (root_ == this) {
    // root: url does not contain name
    url_ = "";
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


const Value Object::list() const {
  ListValue res;
  const_string_iterator it, end = children_.end();
  for (it = children_.begin(); it != end; ++it) {
    Object * obj;
    if (children_.get(*it, &obj)) {
      if (!obj->kind_of(Alias)) {
        // do not list alias (Alias are used as internal helpers and do not
        // need to be advertised) ?
        if (obj->children_.empty()) {
          res.push_back(obj->name_);
        } else {
          res.push_back(std::string(obj->name_).append("/"));
        }
      }
    }
  }
  return res;
}

const Value Object::list_with_type() const {
  ListValue list;
  const_string_iterator it, end = children_.end();
  for (it = children_.begin(); it != end; ++it) {
    ListValue name_with_type;
    Object * obj;
    if (children_.get(*it, &obj)) {
      if (!obj->kind_of(Alias)) {
        // do not list alias (Alias are used as internal helpers and do not
        // need to be advertised) ?
        if (obj->children_.empty()) {
          name_with_type.push_back(obj->name_);
        } else {
          name_with_type.push_back(std::string(obj->name_).append("/"));
        }
        name_with_type.push_back(obj->type_with_current_value());
        list.push_back(name_with_type);
      }
    }
  }
  return list;
}

const Value Object::type_with_current_value() {
  Value type = type_;
  
  if (type.is_string()) {
    // meta type is string = just information (not callable)
    return type;
  }
  
  if (!type.is_list()) {
    // make sure type is a ListValue
    return ErrorValue(INTERNAL_SERVER_ERROR, "Invalid meta type. Should be a list (found '").append(type.type_tag()).append("').");
  }
  
  if (!type[0].is_any() && !type[0].is_nil()) {
    // get current value
    Value current = trigger(gNilValue);
    
    if (current.is_nil()) {
      // current type cannot be queried. Leave dummy value.
    } else if (current.type_id() != type[0].type_id()) {
      // make sure current value type is compatible with type
      return ErrorValue(INTERNAL_SERVER_ERROR, "Current value type not matching meta type (expected '").append(
          type[0].type_tag()).append(
          "' found '").append(current.type_tag()).append("').");
    } else {
      type.set_value_at(0, current);
    }
  
  }
  return type;
}


void Object::tree(size_t base_length, Value *tree) const {
  const_string_iterator it, end = children_.end();
  for (it = children_.begin(); it != end; ++it) {
    Object * obj;
    if (children_.get(*it, &obj)) {
      if (!obj->kind_of(Alias)) {
        // do not list alias (Alias are used as internal helpers and
        // do not need to be advertised) ?
        tree->push_back(obj->url().substr(base_length));
        obj->tree(base_length, tree);
      }
    }
  }
}

} // oscit

#include "root.h"
#include "alias.h"
namespace oscit {
  
Object::~Object()
{  
  // notify parent and root
  if (parent_) parent_->release(this);
  
  for(std::list<Alias*>::iterator it = mAliases.begin(); it != mAliases.end(); it++) {
    // to avoid notification to this dying object
    (*it)->unlinkOriginal();
    delete *it;
  }
  
  clear();
}


/** Inform the object of an alias to be destroyed on destruction. */
void Object::registerAlias(Alias * pAlias)
{
  mAliases.push_back(pAlias);
}

/** Inform the object that an alias no longer exists. */
void Object::unregisterAlias(Alias * pAlias)
{
  mAliases.remove(pAlias);
}

/** Free the child from the list of children. */
void Object::release(Object * pChild)
{
  children_.remove_element(pChild);
  if (root_) root_->unregisterObject(pChild);
}

void Object::moved()
{ 
  // 1. get new name from parent, register as child
  if (parent_) parent_->registerChild(this);
  
  registerUrl();
}

void Object::registerChild(Object * pChild)
{
  // 1. reset hash
  children_.remove_element(pChild);
  
  // 2. get valid name
  while (child(pChild->name_))
    pChild->findNextName();
  
  // 3. set hash back
  children_.set(pChild->name_,pChild);
}

void Object::registerUrl()
{
  Object * obj;
  string_iterator it;
  string_iterator end = children_.end();
  
  // 1. rebuild url
  if (parent_) {
    // build fullpath
    url_ = std::string(parent_->url()).append("/").append(name_);
    if (parent_->root_) parent_->root_->registerObject(this);
  } else {
    // no parent
    url_ = name_;
    if (root_) root_->unregisterObject(this);
  }
  
  // 3. update children
  for(it = children_.begin(); it != end; it++) {
    if (children_.get(&obj, *it)) obj->registerUrl();
  }
}

void Object::clear()
{
  string_iterator it;
  string_iterator end = children_.end();

  // destroy all children
  for(it = children_.begin(); it != end; it++) {
    Object * child;
    if (children_.get(&child, *it)) {
      // to avoid 'release' call (would alter children_)
      child->parent_ = NULL;
      if (root_) root_->unregisterObject(child);
      delete child;
    }
  }
  children_.clear();
}
} // namespace oscit
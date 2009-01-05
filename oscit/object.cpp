#include "robject.h"
#include "root.h"
#include "alias.h"
namespace oscit {
  
Object::~Object()
{  
  // notify parent and root
  if (mParent) mParent->release(this);
  
  for(std::list<Alias*>::iterator it = mAliases.begin(); it != mAliases.end(); it++) {
    // to avoid notification to this dying object
    (*it)->clear_original();
    delete *it;
  }
  
  clear();
}


/** Inform the object of an alias to be destroyed on destruction. */
void Object::register_alias(Alias * pAlias)
{
  mAliases.push_back(pAlias);
}

/** Inform the object that an alias no longer exists. */
void Object::unregister_alias(Alias * pAlias)
{
  mAliases.remove(pAlias);
}

/** Free the child from the list of children. */
void Object::release(Object * pChild)
{
  mChildren.remove_element(pChild);
  if (mRoot) mRoot->unregister_object(pChild);
}

void Object::moved()
{ 
  // 1. get new name from parent, register as child
  if (mParent) mParent->register_child(this);
  
  register_url();
}

void Object::register_child(Object * pChild)
{
  // 1. reset hash
  mChildren.remove_element(pChild);
  
  // 2. get valid name
  while (child(pChild->mName))
    pChild->next_name();
  
  // 3. set hash back
  mChildren.set(pChild->mName,pChild);
}

void Object::register_url()
{
  Object * obj;
  string_iterator it;
  string_iterator end = mChildren.end();
  
  // 1. rebuild url
  if (mParent) {
    // build fullpath
    mUrl = std::string(mParent->url()).append("/").append(mName);
    if (mParent->mRoot) mParent->mRoot->register_object(this);
  } else {
    // no parent
    mUrl = mName;
    if (mRoot) mRoot->unregister_object(this);
  }
  
  // 3. update children
  for(it = mChildren.begin(); it != end; it++) {
    if (mChildren.get(&obj, *it)) obj->register_url();
  }
}

void Object::clear()
{
  string_iterator it;
  string_iterator end = mChildren.end();

  // destroy all children
  for(it = mChildren.begin(); it != end; it++) {
    Object * child;
    if (mChildren.get(&child, *it)) {
      // to avoid 'release' call (would alter mChildren)
      child->mParent = NULL;
      if (mRoot) mRoot->unregister_object(child);
      delete child;
    }
  }
  mChildren.clear();
}
} // namespace oscit
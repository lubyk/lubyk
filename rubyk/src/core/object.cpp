#include "object.h"
#include "root.h"
#include "alias.h"

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
  if (mRoot) mRoot->remove_object(pChild);
}

void Object::moved()
{
  Object * obj;
  std::list<std::string> keys = mChildren.keys(); // copy keys because mChildren is modified in for loop...
  string_iterator it;
  string_iterator end = keys.end();
  
  if (mParent)
    mParent->child_moved(this);
  else
    rebuild_url();
  
  for(it = keys.begin(); it != end; it++) {
    if (mChildren.get(&obj, *it)) obj->moved();
  }
  
}

void Object::child_moved(Object * pChild)
{
  mChildren.remove_element(pChild);
  
  while (child(pChild->mName))
    pChild->next_name();
  
  mChildren.set(pChild->mName,pChild);
  
  pChild->rebuild_url();
  
  if (mRoot) mRoot->object_moved(pChild);
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
      if (mRoot) mRoot->remove_object(child);
      delete child;
    }
  }
  mChildren.clear();
}
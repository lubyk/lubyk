#include "object.h"
#include "root.h"

/** Free the child from the list of children. */
void Object::release(Object * pChild)
{
  mChildren.remove_element(pChild);
  if (mRoot) mRoot->remove_object(pChild);
}

void Object::moved()
{
  string_iterator it;
  string_iterator end = mChildren.end();
  Object * obj;
  
  if (mParent)
    mParent->child_moved(this);
  else
    rebuild_url();
  
  for(it = mChildren.begin(); it != end; it++) {
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
      mRoot->remove_object(child);
      delete child;
    }
  }
  mChildren.clear();
}
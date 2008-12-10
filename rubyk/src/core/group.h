#ifndef _GROUP_H_
#define _GROUP_H_
#include "root.h"
#include "alias.h"

class Group : public Root
{
public:
  bool init (const Value& p);
  
  virtual const Value inspect(const Value& val) ;
  
  virtual void bang(const Value& val);
  
  
  /** Expose an internal object as a method of the group node. */
  virtual void expose(const char * pName, Object * pObject)
  {
    Alias * alias = new Alias(pName, pObject);

    alias->mParent = this;
    alias->mRoot   = mRoot;

    alias->moved();
  }
  
protected:
  /** Actual adoption. Adopt objects in the new namespace (branch). */
  virtual void do_adopt(Object * pObject)
  {
    mBranch.do_adopt(pObject);
  }
private:
  Root mBranch;
};
#endif
#ifndef _OSCIT_ALIAS_H_
#define _OSCIT_ALIAS_H_

#include "oscit/root.h"

namespace oscit {
/** This object triggers another object's trigger when called. It's used in Group to expose functionalities. */
class Alias : public Object
{
public:
  Alias() : Object(""), mOriginal(NULL) {}
  
  Alias(const char *       pName, Object * pObject) : Object(pName, pObject->mTypeTagString.c_str()), mOriginal(pObject) 
  {
    // We register so that the alias dies with the original object.
    mOriginal->register_alias(this);
  }
  
  Alias(const std::string& pName, Object * pObject) : Object(pName, pObject->mTypeTagString.c_str()), mOriginal(pObject) 
  {
    // We register so that the alias dies with the original object.
    mOriginal->register_alias(this);
  }
  
  virtual ~Alias()
  {
    // We unregister to tell the object that it should not delete this alias on destruction.
    if (mOriginal) mOriginal->unregister_alias(this);
  }
  
  /** Class signature. */
  virtual uint type()
  {
    return H("Alias");
  }
  
  virtual const Values trigger (const Values val)
  {
    return mOriginal ? mOriginal->trigger(val) : NULL;
  }
  
  void clear_original()
  {
    mOriginal = NULL;
  }
  
  /** Set new original object from url. */
  void set_original(const std::string& pUrl)
  {
    Object * original = mRoot->find(pUrl);
    if (original) set_original(original);
  }
  
  /** Set new original object from an object pointer. */
  void set_original(Object * pObject)
  {
    if (mOriginal) mOriginal->unregister_alias(this);
    mOriginal = pObject;
    if (mOriginal) mOriginal->register_alias(this);
  }
protected:
  Object * mOriginal; /**< Original object pointed to by the alias. */
};
} // namespace oscit
#endif // _OSCIT_ALIAS_H_
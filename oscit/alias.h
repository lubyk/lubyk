#ifndef _OSCIT_ALIAS_H_
#define _OSCIT_ALIAS_H_

#include "oscit/root.h"

namespace oscit {
/** This object triggers another object's trigger when called. It's used in Group to expose functionalities. */
class Alias : public Object
{
public:
  Alias() : Object(""), original_(NULL) {}
  
  Alias(const char * name, Object * object) : Object(name, object->typeTagStr_.c_str()), original_(object) 
  {
    // We register so that the alias dies with the original object.
    original_->registerAlias(this);
  }
  
  Alias(const std::string& name, Object * object) : Object(name, object->typeTagStr_.c_str()), original_(object) 
  {
    // We register so that the alias dies with the original object.
    original_->registerAlias(this);
  }
  
  virtual ~Alias()
  {
    // We unregister to tell the object that it should not delete this alias on destruction.
    if (original_) original_->unregisterAlias(this);
  }
  
  /** Class signature. */
  virtual uint type()
  {
    return H("Alias");
  }
  
  virtual const Value trigger (const Value& val)
  {
    return original_ ? original_->trigger(val) : gNilValue;
  }
  
  void unlinkOriginal()
  {
    original_ = NULL;
  }
  
  /** Set new original object from url. */
  void set_original(const std::string& pUrl)
  {
    Object * original = root_->find(pUrl);
    if (original) set_original(original);
  }
  
  /** Set new original object from an object pointer. */
  void set_original(Object * object)
  {
    if (original_) original_->unregisterAlias(this);
    original_ = object;
    if (original_) original_->registerAlias(this);
  }
protected:
  Object * original_; /**< Original object pointed to by the alias. */
};
} // namespace oscit
#endif // _OSCIT_ALIAS_H_
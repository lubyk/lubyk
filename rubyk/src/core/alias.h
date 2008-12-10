#ifndef _PROXY_METHOD_H_
#define _PROXY_METHOD_H_
#include "object.h"

/** This object triggers another object's trigger when called. It's used in Group to expose functionalities. */
class Alias : public Object
{
public:
  Alias(const char *       pName, Object * pObject) : Object(pName), mOriginal(pObject) 
  {
    // We register so that the alias dies with the original object.
    mOriginal->register_alias(this);
  }
  
  Alias(const std::string& pName, Object * pObject) : Object(pName), mOriginal(pObject) 
  {
    // We register so that the alias dies with the original object.
    mOriginal->register_alias(this);
  }
  
  virtual ~Alias()
  {
    // We unregister to tell the object that it should not delete this alias on destruction.
    if (mOriginal) mOriginal->unregister_alias(this);
  }
  
  virtual const Value trigger (const Value& val)
  {
    return mOriginal->trigger(val);
  }
  
  void clear_original()
  {
    mOriginal = NULL;
  }
  
private:
  Object * mOriginal; /**< Original object pointed to by the alias. */
};

#endif // _PROXY_METHOD_H_
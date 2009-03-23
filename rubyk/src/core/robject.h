#ifndef _OBJECT_H_
#define _OBJECT_H_
#include "oscit/object.h"
#include "oscit/root.h"

class Planet;

class RObject : public oscit::Object
{
public:
  RObject(const char * pTypeTagStr) : oscit::Object(pTypeTagStr), mPlanet(NULL) {}
  
  RObject(const char * pTypeTagStr, Planet * pPlanet) : oscit::Object(pTypeTagStr), mPlanet(pPlanet) {}

  RObject(const char * pTypeTagStr, const char * pName) : oscit::Object(pTypeTagStr, pName), mPlanet(NULL) {}

  RObject(const char * pTypeTagStr, const std::string& pName) : oscit::Object(pTypeTagStr, pName), mPlanet(NULL) {}

  RObject(const char * pTypeTagStr, Object * pParent) : oscit::Object(pTypeTagStr, pParent) {}

  RObject(const char * pTypeTagStr, Object * pParent, const char * pName) : oscit::Object(pTypeTagStr, pParent, pName) {}

  RObject(const char * pTypeTagStr, Object * pParent, const std::string& pName) : oscit::Object(pTypeTagStr, pParent, pName) {}

  RObject(const char * pTypeTagStr, Object& pParent) : oscit::Object(pTypeTagStr, pParent) {}

  RObject(const char * pTypeTagStr, Object& pParent, const char * pName) : oscit::Object(pTypeTagStr, pParent, pName) {}

  RObject(const char * pTypeTagStr, Object& pParent, const std::string& pName) : oscit::Object(pTypeTagStr, pParent, pName) {}
  
  virtual void set_root(oscit::Root * pRoot)
  {  
    oscit::Object::set_root(pRoot);
    if (pRoot) mPlanet = (Planet*)(pRoot->mGround);
  }
  
protected:
  
  Planet * mPlanet;  /**< Running planet. */
};

#endif // _OBJECT_H_
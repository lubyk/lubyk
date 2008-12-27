#ifndef _OBJECT_H_
#define _OBJECT_H_
#include "oscit/object.h"
#include "oscit/root.h"

class Planet;

class RObject : public oscit::Object
{
public:
  RObject() : mPlanet(NULL) {}
  
  RObject(Planet * pPlanet) : mPlanet(pPlanet) {}

  RObject(const char * pName) : oscit::Object(pName), mPlanet(NULL) {}

  RObject(const std::string& pName) : oscit::Object(pName), mPlanet(NULL) {}

  RObject(Object * pParent) : oscit::Object(pParent) {}

  RObject(Object * pParent, const char * pName) : oscit::Object(pParent, pName) {}

  RObject(Object * pParent, const std::string& pName) : oscit::Object(pParent, pName) {}

  RObject(Object& pParent) : oscit::Object(pParent) {}

  RObject(Object& pParent, const char * pName) : oscit::Object(pParent, pName) {}

  RObject(Object& pParent, const std::string& pName) : oscit::Object(pParent, pName) {}
  
  virtual void set_root(oscit::Root * pRoot)
  {  
    oscit::Object::set_root(pRoot);
    if (pRoot) mPlanet = (Planet*)(pRoot->mGround);
  }
  
protected:
  
  Planet * mPlanet;  /**< Running planet. */
};

#endif // _OBJECT_H_
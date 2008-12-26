#ifndef _OBJECT_H_
#define _OBJECT_H_
#include "oscit/object.h"

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
  
  /** Incarnation on a planet. Normally this is done by providing an ancestry tree by using: 'set_parent'. */
  void set_planet(Planet * pPlanet)
  {
    mPlanet = pPlanet;
  }
  
protected:  
  //friend class Planet;
  
  /** Actual adoption. Adopt objects in the new namespace (branch). */
  virtual void do_adopt(RObject * pObject)
  {
    oscit::Object::do_adopt(pObject);
    pObject->mPlanet = mPlanet;
  }
  
  Planet * mPlanet;  /**< Running planet. */
};

#endif // _OBJECT_H_
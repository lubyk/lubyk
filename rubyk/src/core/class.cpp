#include "class.h"
#include "alias.h"


/** Build all inlets for an object from prototypes. */
void Class::make_inlets(Node * pObj)
{
  std::list<InletPrototype>::iterator it;
  std::list<InletPrototype>::iterator end = mInletPrototypes.end();
  Object * inlets = pObj->adopt(new Object("in"));
  Inlet * inlet;
  uint i = 0;
  char buffer[50];
  
  for (it = mInletPrototypes.begin(); it != end; it++) {
    inlet = inlets->adopt(new Inlet(pObj, *it));
    snprintf(buffer, 50, "%u", ++i);
    mRoot->set_alias(std::string(inlets->url()).append("/").append(buffer), inlet); // in/1
  }
}

/** Build all inlets for an object from prototypes. */
void Class::make_outlets(Node * pObj)
{
  std::list<OutletPrototype>::iterator it;
  std::list<OutletPrototype>::iterator end = mOutletPrototypes.end();
  Object * outlets = pObj->adopt(new Object("out"));
  Object * method;
  Outlet * outlet;
  uint i = 0;
  char buffer[50];
  
  for (it = mOutletPrototypes.begin(); it != end; it++) {
    outlet = outlets->adopt(new Outlet(pObj, *it));
    snprintf(buffer, 50, "%u", ++i);
    method = outlet->child("link");
    if (method) mRoot->set_alias(std::string(outlets->url()).append("/").append(buffer).append("/link"), method); // out/1/link
    method = outlet->child("unlink");
    if (method) mRoot->set_alias(std::string(outlets->url()).append("/").append(buffer).append("/unlink"), method); // out/1/unlink
  }
}
#include "class.h"
#include "oscit/alias.h"


/** Build all inlets for an object from prototypes. */
void Class::make_inlets(Node * pObj)
{
  std::list<InletPrototype>::iterator it;
  std::list<InletPrototype>::iterator end = mInletPrototypes.end();
  oscit::Object * inlets = pObj->adopt(new Object("in"));
  
  for (it = mInletPrototypes.begin(); it != end; it++)
    inlets->adopt(new Inlet(pObj, *it));
}

/** Build all inlets for an object from prototypes. */
void Class::make_outlets(Node * pObj)
{
  std::list<OutletPrototype>::iterator it;
  std::list<OutletPrototype>::iterator begin = mOutletPrototypes.begin();
  std::list<OutletPrototype>::iterator end   = mOutletPrototypes.end();
  oscit::Object * outlets = pObj->adopt(new Object("out"));
  oscit::Object * method;
  Outlet * outlet;
  
  for (it = begin; it != end; it++) {
    outlet = outlets->adopt(new Outlet(pObj, *it));
    if (it == begin) {
      // alias for first "link" in out/link
      method = outlet->child("link");
      if (method) outlets->adopt(new oscit::Alias("link", method)); // out/link
      method = outlet->child("unlink");
      if (method) outlets->adopt(new oscit::Alias("unlink", method)); // out/unlink
    }
  }
}
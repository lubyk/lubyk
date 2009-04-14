#include "class.h"
#include "oscit/alias.h"


/** Build all inlets for an object from prototypes. */
void Class::make_inlets(Node *object)
{
  std::list<InletPrototype>::iterator it;
  std::list<InletPrototype>::iterator end = inlet_prototypes_.end();
  Object * inlets = object->adopt(new Object("in"));
  
  for (it = inlet_prototypes_.begin(); it != end; it++)
    inlets->adopt(new Inlet(object, *it));
}

/** Build all inlets for an object from prototypes. */
void Class::make_outlets(Node *object)
{
  std::list<OutletPrototype>::iterator it;
  std::list<OutletPrototype>::iterator begin = outlet_prototypes_.begin();
  std::list<OutletPrototype>::iterator end   = outlet_prototypes_.end();
  Object * outlets = object->adopt(new Object("out"));
  //FIX: Object * method;
  Outlet * outlet;
  
  for (it = begin; it != end; it++) {
    outlet = outlets->adopt(new Outlet(object, *it));
    if (it == begin) {
      // alias for first "link" in out/link
      //FIX: method = (Object*)outlet->child("link");
      //FIX: if (method) outlets->adopt(new Alias("link", method)); // out/link ---> out/counter/link
      //FIX: method = (Object*)outlet->child("unlink");
      //FIX: if (method) outlets->adopt(new Alias("unlink", method)); // out/unlink ---> out/counter/unlink
    }
  }
}
#ifndef _OUTLET_H_
#define _OUTLET_H_

#include "slot.h"
#include "values.h"

/** Prototype constructor for Inlets. */
struct OutletPrototype
{
  OutletPrototype(const char * pName, uint pType, const char * pInfo) : mName(pName), mType(pType), mInfo(pInfo) {}
  const char *   mName;
  uint           mType;
  const char *   mInfo;
};


class Outlet : public Slot {
public:  
  Outlet (Node * pNode) : Slot(pNode) 
  {
    register_in_node();
  }
  
  Outlet (Node * pNode, uint pType) : Slot(pNode, pType) 
  {
    register_in_node();
  }
  
  Outlet (const std::string& pName, Node * node, uint pType) : Slot(pName, node, pType)
  {
    register_in_node();
  }
  
  /** Prototype based constructor. */
  Outlet (Node * pNode, const OutletPrototype& pProto) : Slot(pProto.mName, pNode, pProto.mType)
  {
    set_info(pProto.mInfo);
    register_in_node();
  }
  
  virtual ~Outlet() 
  {
    unregister_in_node();
  }
  
  /** Inform the node about the existence of this outlet (direct callback). */
  void register_in_node();
  
  /** Inform the node that this outlet is about to disappear. */
  void unregister_in_node();
  
  /** Class signature. */
  virtual uint type()
  {
    return H("Outlet");
  }
  
  /** Send the signal to all connections. Do nothing if there are no connections. */
  void send (const Value& s);
};

#endif

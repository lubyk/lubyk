#ifndef _INLET_H_
#define _INLET_H_
#include "slot.h"
#include "values.h"

class Node;

typedef void(*inlet_method_t)(Node * node, const Value& val);

/** Prototype constructor for Inlets. */
struct InletPrototype
{
  InletPrototype(const char * pName, uint pType, inlet_method_t pMethod, const char * pInfo) : mName(pName), mType(pType), mMethod(pMethod), mInfo(pInfo) {}
  const char *   mName;
  uint           mType;
  inlet_method_t mMethod;
  const char *   mInfo;
};

class Inlet : public Slot {
public:
  /** Constructor used for testing. */
  Inlet (Node * node, inlet_method_t pMethod, uint pType) : Slot(node, pType), mMethod(pMethod) 
  {
    register_in_node();
  }
  
  Inlet (Node * node, inlet_method_t pMethod) : Slot(node), mMethod(pMethod) 
  {
    register_in_node();
  }
  
  /** Prototype based constructor. */
  Inlet (Node * pNode, const InletPrototype& pProto) : Slot(pProto.mName, pNode, pProto.mType), mMethod(pProto.mMethod)
  {
    set_info(pProto.mInfo);
    register_in_node();
  }
  
  virtual ~Inlet()
  {
    unregister_in_node();
  }
  
  /** Inform the node about the existence of this outlet (direct callback). */
  void register_in_node();
  
  /** Inform the node that this outlet is about to disappear. */
  void unregister_in_node();
  
  /** The operation to be executed on call. If 'val' is not Nil, send to inlet. */
  virtual const Value trigger (const Value& val)
  {
    if (val.is_nil())
      return this->Object::trigger(val);
    else {
      receive(val);
      return gNilValue;
    }
  }
  
  /** Class signature. */
  virtual uint type()
  {
    return H("Inlet");
  }
  
  /** Receive a value. */
  void receive (const Value& val);
  
  /** Create a callback for an inlet. */
  template <class T, void(T::*Tmethod)(const Value& val)>
  static void cast_method (Node * receiver, const Value& val)
  {
    (((T*)receiver)->*Tmethod)(val);
  }
  
private:
  inlet_method_t mMethod;        /**< Method to set a new value. */
};

#endif
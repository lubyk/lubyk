#ifndef _SLOT_H_
#define _SLOT_H_
#include "ordered_list.h"
#include "object.h"
#include "method.h"
#include "rubyk_types.h"

class Node;

/** Inlets and outlets of nodes are Slots. 
  * 
  * Slots are responsible for managing connections and passing values between objects. When a slot is created,
  * it holds a reference to it's master node 'receiver' and to a pointer to a member method that should be
  * triggered to set/get the value for the given in/out let. */
class Slot : public Object
{
public:
  Slot (Node * pNode) : mNode(pNode), mType(AnyValue) 
  {
    create_methods();
  }
  
  Slot (Node * pNode, uint pType) : mNode(pNode), mType(pType)
  {
    create_methods();
  }
  
  Slot (const char * pName, Node * pNode, uint pType) : Object(pName), mNode(pNode), mType(pType) 
  {
    create_methods();
  }

  virtual ~Slot ();
  
  /** Set slot id (position in containing node) */
  void set_id (int pId);
  
  /** Add a bi-directional connection to another slot. */
  bool connect (Slot * pOther);
  
  /** Remove a connection to another slot. */
  void disconnect (Slot * pOther);
  
  /** Sort slots by rightmost node and rightmost position in the same node. */
  bool operator>= (const Slot& pOther) const;
  
  void * node () { return mNode; }
  
protected:
  /** Make a one-way connection to another slot. 
    * Create a connection if the type of the other slot is compatible. */
  bool add_connection (Slot * pOther);
  
  /** Remove a one-way connection to another slot. */
  void remove_connection (Slot * pOther);
  
  void create_methods()
  {
    Method * m = adopt(new Method("link", this, &Method::cast_method<Slot, &Slot::link>));
    m->set_info("Create a link / list links.");
  }
  
  // /m/out/counter/link  --> list links
  // /m/out/counter/link /n/in/tempo  --> create a link
  const Value link(const Value& val);
  
protected:
  Node * mNode; /**< Containing node.      */
  int    mId;   /**< Position in the node. */
  uint  mType;  /**< slot type signature.  */
  
  OrderedList<Slot*> mConnections; /**< connections are kept sorted, so that we always send values to inlets
    that are rightmost (less important, no bang) first. */
};

#endif
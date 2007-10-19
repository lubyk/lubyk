#ifndef _SLOT_H_
#define _SLOT_H_

#include "../templates/orderedList.h"

class Node;
/** Inlets and outlets of nodes are Slots. 
  * 
  * Slots are responsible for managing connections and passing values between objects. When a slot is created,
  * it holds a reference to it's master node 'receiver' and to a pointer to a member method that should be
  * triggered to set/get the value for the given in/out let. */
class Slot
{
  public:
    Slot (void* node) : mNode(node) {}
  
    virtual ~Slot () {};

    /* PUBLIC METHODS */
    
    /** Set slot id (position in containing node) */
    void setId (int pId);
    
    /** Add a bi-directional connection to another slot. */
    void connect (Slot * pOther);
    
    /** Remove a connection to another slot. */
    void disconnect (Slot * pOther);
    
    /** Sort slots by rightmost node and rightmost position in the same node. */
    inline bool operator>= (const Slot& pOther) const;
    
  protected:
    /** Make a one-way connection to another slot. */
    void add_connection (Slot * pOther);
    
    /** Remove a one-way connection to another slot. */
    void remove_connection (Slot * pOther);
    
    /* ATTRIBUTES */
    void* mNode; /**< containing node.      */
    int   mId;   /**< position in the node. */

    OrderedList<Slot> mConnections; /**< connections are kept sorted, so that we always send values to inlets
                                         that are rightmost (less important, no bang) first. */
};

#endif
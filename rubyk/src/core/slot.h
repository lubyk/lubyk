#ifndef _SLOT_H_
#define _SLOT_H_
#include "ordered_list.h"
#include "object.h"
#include "method.h"

class Node;

/** Inlets and outlets of nodes are Slots. 
  * 
  * Slots are responsible for managing connections and passing values between objects. When a slot is created,
  * it holds a reference to it's master node 'receiver' and to a pointer to a member method that should be
  * triggered to set/get the value for the given in/out let. */
class Slot : public Object
{
public:
  Slot(Node *node, TypeTagID type_tag_id) : Object(type_tag_id), node_(node) {
    create_methods();
  }
  
  Slot(Node *node, const char * pName, TypeTagID type_tag_id) : Object(name, type_tag_id), node_(pNode) {
    create_methods();
  }
  
  Slot(Node *node, const std::string &name, TypeTagID type_tag_id) : Object(name, type_tag_id), node_(pNode) {
    create_methods();
  }
  
  virtual ~Slot();
  
  /** Set slot id (position in containing node) */
  void set_id(int id);
  
  /** Add a bi-directional connection to another slot. */
  bool connect(Slot *slot);
  
  /** Remove a connection to another slot. */
  void disconnect(Slot *slot);
  
  /** Sort slots by rightmost node and rightmost position in the same node. */
  bool operator>=(const Slot &slot) const;
  
  void *node() { return node_; }
  
protected:
  /** Make a one-way connection to another slot. 
    * Create a connection if the type of the other slot is compatible. */
  bool add_connection (Slot * slot);
  
  /** Remove a one-way connection to another slot. */
  void remove_connection (Slot * slot);
  
  /** Create 'link' and 'unlink' methods. */
  void create_methods() {
    Method * m = adopt(new Method("link",   this, &Method::cast_method<Slot, &Slot::link>));
    m->set_info("Create a link / list links.");
    m = adopt(new Method("unlink", this, &Method::cast_method<Slot, &Slot::unlink>));
    m->set_info("Remove a link / list links.");
  }
  
  // /m/out/counter/link  --> list links
  // /m/out/counter/link /n/in/tempo  --> create a link
  /** Create a link. */
  const Value link(const Value val)
  {
    return change_link(val, 'c');
  }
  
  /** Delete a link. */
  const Value unlink(const Value val)
  {
    return change_link(val, 'd');
  }
  
  
protected:  
  
  /** If operation is 'c': create a new link, else unlink. */
  const Value change_link(const Value val, unsigned char operation);
  
  Node * node_;   /**< Containing node.      */
  int    id_;     /**< Position in the node. */
  
  OrderedList<Slot*> connections_; /**< connections are kept sorted, so that we always send values to inlets
    that are rightmost (less important, no bang) first. */
};

#endif
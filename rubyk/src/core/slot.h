#ifndef _SLOT_H_
#define _SLOT_H_
#include "ordered_list.h"
#include "object.h"

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
  
  Slot(Node *node, const char *name, TypeTagID type_tag_id) : Object(name, type_tag_id), node_(node) {
    create_methods();
  }
  
  Slot(Node *node, const std::string &name, TypeTagID type_tag_id) : Object(name, type_tag_id), node_(node) {
    create_methods();
  }
  
  virtual ~Slot();
  
  /** Set slot id (position in containing node) */
  void set_id(int id) {
    id_ = id;
    sort_incoming_connections();
  }
  
  /** Add a bi-directional connection to another slot. */
  bool connect(Slot *slot);
  
  /** Remove a bi-directional connection to another slot. */
  void disconnect(Slot *slot);
  
  /** Sort slots by rightmost node and rightmost position in the same node. */
  bool operator>=(const Slot &slot) const;
  
  void *node() { return node_; }
  
  /** Connections pointing out of this slot should reorder (an inlet id changed or its node changed position). */
  void sort_connections(Slot *changed) {
    connections_.sort_object(changed);
  }
  
  void sort_incoming_connections() {
    LinkedList<Slot*> * iterator = connections_.begin();

    while(iterator) {
      iterator->obj->sort_connections(this);
      iterator = iterator->next;
    }
  }
protected:
  /** Make a one-way connection to another slot. 
    * Create a connection if the type of the other slot is compatible. */
  bool add_connection(Slot *slot);
  
  /** Remove a one-way connection to another slot. */
  void remove_connection(Slot *slot);
  
  /** Create 'link' and 'unlink' methods. */
  void create_methods() {
    adopt(new TMethod<Slot, &Slot::link>(this, "link", H("s"), "Create a link to the provided url."));
    adopt(new TMethod<Slot, &Slot::unlink>(this, "unlink", H("s"), "Remove links to the provided url."));
    adopt(new TMethod<Slot, &Slot::list>(this, "list", H("*"), "Return a list of linked urls."));
  }
  
  /** Create a link. 
   *  example: /m/out/counter/link /n/in/tempo  --> create a link
   */
  const Value link(const Value &val) {
    return change_link('c', val);
  }
  
  /** Delete a link. */
  const Value unlink(const Value &val) {
    return change_link('d', val);
  }
  
  /** List all links. */
  const Value list(const Value &val) {
    LinkedList<Slot*> * iterator = connections_.begin();
    Value res;
    while(iterator) {
      res.push_back(iterator->obj->url());
      iterator = iterator->next;
    }
    return res;
  }
  
  /** If operation is 'c': create a new link, else unlink. */
  const Value change_link(unsigned char operation, const Value &val);
  
  Node * node_;   /**< Containing node.      */
  int    id_;     /**< Position in the node. */
  
  OrderedList<Slot*> connections_; /**< connections are kept sorted, so that we always send values to inlets
    that are rightmost (less important, no bang) first. */
};

#endif
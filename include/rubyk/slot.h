/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
   Copyright (c) 2007-2010 by Gaspard Bucher - Buma (http://teti.ch).

  ------------------------------------------------------------------------------

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

  ==============================================================================
*/

#ifndef RUBYK_INCLUDE_RUBYK_SLOT_H_
#define RUBYK_INCLUDE_RUBYK_SLOT_H_

#include "rubyk/oscit.h"
#include "rubyk/ordered_list.h"

class Node;

/** Inlets and outlets of nodes are Slots. 
  * 
  * Slots are responsible for managing connections and passing values between objects. When a slot is created,
  * it holds a reference to it's master node 'receiver' and to a pointer to a member method that should be
  * triggered to set/get the value for the given in/out let. */
class Slot : public Object
{
public:
  TYPED("Object.Slot")
  
  Slot(Node *node, const Value &type) : Object(type), node_(node) {
    create_methods();
  }
  
  Slot(Node *node, const char *name, const Value &type) : Object(name, type), node_(node) {
    create_methods();
  }
  
  Slot(Node *node, const std::string &name, const Value &type) : Object(name, type), node_(node) {
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
  
  /** Sort slots by rightmost node and rightmost position in the same node. */
  bool operator>=(const Slot &slot) const;
  
  inline Node *node() { return node_; }
  
  /** Connections pointing out of this slot should reorder (an inlet id changed or its node changed position). */
  void sort_connections() {
    connections_.sort();
  }

  void sort_incoming_connections() {
    LinkedList<Slot*> * iterator = connections_.begin();

    while(iterator) {
      iterator->obj->sort_connections();
      iterator = iterator->next;
    }
  }

protected:
  /** Make a one-way connection to another slot. 
    * Create a connection if the type of the other slot is compatible. */
  bool add_connection(Slot *slot);
  
  /** Remove a one-way connection to another slot. */
  void remove_connection(Slot *slot);
  
  /** Create 'list' method. */
  void create_methods() {
    adopt(new TMethod<Slot, &Slot::list>(this, "list", NilIO("Return a list of linked urls.")));
  }

  /** If operation is 'c': create a new link, else unlink. */
  const Value change_link(unsigned char operation, const Value &val);
  
  /** Containing node.
   */
  Node *node_;
  
  /** Position in the node.
   */
  int id_;
  
  OrderedList<Slot*> connections_; /**< connections are kept sorted, so that we always send values to inlets
    that are rightmost (less important, no bang) first. */
};

#endif // RUBYK_INCLUDE_RUBYK_SLOT_H_
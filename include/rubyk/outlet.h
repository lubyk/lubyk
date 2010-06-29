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

#ifndef RUBYK_INCLUDE_RUBYK_OUTLET_H_
#define RUBYK_INCLUDE_RUBYK_OUTLET_H_

#include "oscit/object.h"
#include "rubyk/c_ordered_list.h"

namespace rk {

class Inlet;
class Node;

/** Prototype constructor for Inlets. */
struct OutletPrototype
{
  OutletPrototype(const char *name, const Value &type) : name_(name), type_(type) {}
  std::string name_;
  Value       type_;
};

class Outlet : public Object {
public:
  TYPED("Object.Outlet")

  Outlet(Node *node, const Value &type) :
      Object(type), node_(node) {
    register_in_node();
  }

  Outlet(Node *node, const std::string &name, const Value &type) :
      Object(name, type), node_(node) {
    register_in_node();
  }

  Outlet(Node *node, const char *name, const Value &type) :
      Object(name, type), node_(node) {
    register_in_node();
  }

  /** Prototype based constructor. */
  Outlet(Node *node, const OutletPrototype &prototype) :
      Object(prototype.name_, prototype.type_), node_(node) {
    register_in_node();
  }

  /** On destruction, unregister in node and remove connections.
   */
  virtual ~Outlet();

  /** Inform the node about the existence of this outlet (direct callback). */
  void register_in_node();

  /** Inform the node that this outlet is about to disappear. */
  void unregister_in_node();

  /** Send the signal to all connections. Do nothing if there are no connections. */
  void send(const Value &val);

  /** Return a hash representation of an Outlet.
   * The result is an array of linked targets.
   */
  virtual void insert_in_hash(Value *result);

  /** Create a link.
   * example: /m/out/counter/link /n/in/tempo  --> create a link
   */
  const Value link(const Value &val) {
    return change_link('c', val);
  }

  /** Delete a link. */
  const Value unlink(const Value &val) {
    return change_link('d', val);
  }

  /** Add a bi-directional connection to an inlet.
   */
  bool connect(Inlet *inlet);

  /** Remove a bi-directional connection to an inlet.
   */
  void disconnect(Inlet *inlet);

  /** Make a one-way connection to another inlet.
   * Create a connection if the type of the other inlet is compatible.
   */
  bool add_connection(Inlet *inlet);

  /** Remove a one-way connection to an inlet.
   */
  void remove_connection(Inlet *inlet);

  /** Resort connections (triggered by a connected inlet because the id or node moved).
   */
  void sort_connections();

private:

  /** If operation is 'c': create a new link, else unlink.
   */
  const Value change_link(unsigned char operation, const Value &val);

  /** Containing node.
   */
  Node *node_;

  /** Connections are kept sorted, so that we always send values to inlets
   * that are rightmost first.
   */
  COrderedList<Inlet*> connected_inlets_;

};

} // rk

#endif // RUBYK_INCLUDE_RUBYK_OUTLET_H_

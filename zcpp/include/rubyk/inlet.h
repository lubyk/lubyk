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

#ifndef RUBYK_INCLUDE_RUBYK_INLET_H_
#define RUBYK_INCLUDE_RUBYK_INLET_H_

#include <list>

#include "oscit.h"

namespace rk {

class Outlet;
class Node;

/** An inlet is a special kind of Method that can handle connections from Outlet.
 */
class Inlet : public Method {
public:
  TYPED("Object.Method.Inlet")

  /** Constructor used for testing. */
  // Inlet(Node *node, inlet_method_t method, const Value &type) : Slot(node, type), method_(method) {
  //   register_in_node();
  // }

  /** Create a new object that call a member method when "triggered" and handles connections from Outlet.
   */
  Inlet(Node *node, const char *name, member_method_t method, const Value &attrs) :
      Method(node, name, method, attrs),
      node_(node) {
    register_in_node();
  }

  /** Create a new object that call a member method when "triggered" and handles connections from Outlet.
   */
  Inlet(Node *node, const std::string &name, member_method_t method, const Value &attrs) :
      Method(node, name, method, attrs),
      node_(node) {
    register_in_node();
  }

  /** Prototype based constructor.
   */
  Inlet(Node *node, const MethodPrototype &prototype) :
      Method(node, prototype),
      node_(node) {
    register_in_node();
  }

  /** On destruction, inform master node that we no longer exist so the node can remove this inlet
   * from the list and remove links from outlets.
   */
  virtual ~Inlet();

  /** Inform the node about the existence of this inlet.
   */
  void register_in_node();

  /** Inform the node that this inlet is about to disappear.
   */
  void unregister_in_node();

  /** Set inlet id (position in containing node).
   */
  void set_id(int id) {
    id_ = id;
    sort_incoming_connections();
  }

  /** Sort inlets by rightmost node and rightmost position in the same node.
   */
  bool operator>=(const Inlet &other) const;

  /** Make a one-way connection to another slot (@internal: should only be called by Outlet).
   */
  bool add_connection(Outlet *outlet);

  /** Remove a one-way connection from an outlet (@internal: should only be called by Outlet).
   */
  void remove_connection(Outlet *outlet);

  /** (re)sort connections arriving to this inlet. This is triggered when the
   * node position changes.
   */
  void sort_incoming_connections();

private:

  /** Containing node.
   */
  Node *node_;

  /** Position in the node, used to sort.
   */
  int id_;

  /** Outlets connected.
   */
  CTList<Outlet*> connected_outlets_;

};

} // rk

#endif // RUBYK_INCLUDE_RUBYK_INLET_H_
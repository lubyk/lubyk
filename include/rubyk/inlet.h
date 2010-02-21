/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
   Copyright (c) 2007-2009 by Gaspard Bucher - Buma (http://teti.ch).

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

#include "rubyk/slot.h"

class Inlet;
class Node;

typedef void(*inlet_method_t)(Inlet *node, const Value &val);

/** Prototype constructor for Inlets. */
struct InletPrototype
{
  InletPrototype(const char *name, inlet_method_t method, const Value &type) : name_(name), method_(method), type_(type) {}
  
  std::string    name_;
  inlet_method_t method_;
  Value          type_;
};

class Inlet : public Slot {
public:
  TYPED("Object.Slot.Inlet")
  
  /** Constructor used for testing. */
  Inlet(Node *node, inlet_method_t method, const Value &type) : Slot(node, type), method_(method) {
    register_in_node();
  }
  
  Inlet(Node *node, const char *name, inlet_method_t method, const Value &type) : Slot(node, name, type), method_(method) {
    register_in_node();
  }
  
  /** Prototype based constructor. */
  Inlet(Node *node, const InletPrototype &prototype) : Slot(node, prototype.name_, prototype.type_), method_(prototype.method_) {
    register_in_node();
  }
  
  virtual ~Inlet() {
    unregister_in_node();
  }
  
  /** Inform the node about the existence of this outlet (direct callback). */
  void register_in_node();
  
  /** Inform the node that this outlet is about to disappear. */
  void unregister_in_node();
  
  /** The operation to be executed on call. If 'val' is not Nil, send to inlet. */
  virtual const Value trigger(const Value &val);
    
  /** Receive a value. */
  inline void receive(const Value &val) {
    (*method_)(this, val);     // use functor
  }
  
  /** Create a callback for an inlet. */
  template <class T, void(T::*Tmethod)(const Value &val)>
  static void cast_method(Inlet *inlet, const Value &val) {
    (((T*)inlet->node_)->*Tmethod)(val);
  }
  
  /** Create a callback for an inlet based on a normal accessor. */
  template <class T, const Value(T::*Tmethod)(const Value &val)>
  static void cast_method(Inlet *inlet, const Value &val) {
    (((T*)inlet->node_)->*Tmethod)(val);
    // ignore return value
  }
  
  /** Create a callback for an inlet (method in superclass). */
  template <class R, class T, void(T::*Tmethod)(const Value &val)>
  static void cast_method(Inlet *inlet, const Value &val) {
    (((R*)inlet->node_)->*Tmethod)(val);
  }
  
  /** Create a callback for an inlet based on a normal accessor (method in superclass). */
  template <class R, class T, const Value(T::*Tmethod)(const Value &val)>
  static void cast_method(Inlet *inlet, const Value &val) {
    (((R*)inlet->node_)->*Tmethod)(val);
    // ignore return value
  }
private:
  inlet_method_t method_;        /**< Method to set a new value. */
};

#endif // RUBYK_INCLUDE_RUBYK_INLET_H_
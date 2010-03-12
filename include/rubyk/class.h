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

#ifndef RUBYK_INCLUDE_RUBYK_CLASS_H_
#define RUBYK_INCLUDE_RUBYK_CLASS_H_

#include "rubyk/node.h"
#include "rubyk/planet.h"

#include <list>

namespace rk {

/** This is a helper to prepare prototypes to:
  * 1. create nodes
  * 2. create node methods
  * 3. create node inlets / outlets
  */
class Class : public Object
{
public:
  TYPED("Object.Class")

  Class(const char* name, const Value &type) : Object(name, type) {}

  virtual ~Class() {}

  /** Add a new class method to the class. */
  void add_class_method(const char *name, class_method_t method, const Value &type) {
    adopt(new ClassMethod(name, method, type));
  }

  /** Declare a method. */
  template <class T, const Value(T::*Tmethod)(const Value &val)>
  void add_method(const char *name, const Value &type) {
    method_prototypes_.push_back( MethodPrototype(name, &Method::cast_method<T, Tmethod>, type) );
  }

  /** Declare a method defined in a superclass.
   * template parameters are:
   * R = receiver class
   * T = super class
   * *Tmethod = pointer to member method in T class
   */
  template <class R, class T, const Value(T::*Tmethod)(const Value &val)>
  void add_method(const char *name, const Value &type) {
    method_prototypes_.push_back( MethodPrototype(name, &Method::cast_method<R, T, Tmethod>, type) );
  }

  /** Declare a method without return value.
   */
  template <class T, void(T::*Tmethod)(const Value &val)>
  void add_method(const char *name, const Value &type) {
    method_prototypes_.push_back( MethodPrototype(name, &Method::cast_method<T, Tmethod>, type) );
  }

  /** Declare a method without return value, defined in a superclass.
   */
  template <class R, class T, void(T::*Tmethod)(const Value &val)>
  void add_method(const char *name, const Value &type) {
    method_prototypes_.push_back( MethodPrototype(name, &Method::cast_method<R, T, Tmethod>, type) );
  }

  /** Declare an inlet from a method (less efficient, should be avoided for fast inlets). */
  template <class T, const Value(T::*Tmethod)(const Value &val)>
  void add_inlet(const char *name, const Value &type) {
    inlet_prototypes_.push_back( InletPrototype(name, &Inlet::cast_method<T, Tmethod>, type) );
  }


  /** Declare an inlet. */
  template <class T, void(T::*Tmethod)(const Value &val)>
  void add_inlet(const char *name, const Value &type) {
    inlet_prototypes_.push_back( InletPrototype(name, &Inlet::cast_method<T, Tmethod>, type) );
  }


  /** Declare an inlet from a method in superclass (less efficient, should be avoided for fast inlets). */
  template <class R, class T, const Value(T::*Tmethod)(const Value &val)>
  void add_inlet(const char *name, const Value &type) {
    inlet_prototypes_.push_back( InletPrototype(name, &Inlet::cast_method<R, T, Tmethod>, type) );
  }


  /** Declare an inlet from a method in superclass.
   */
  template <class R, class T, void(T::*Tmethod)(const Value &val)>
  void add_inlet(const char *name, const Value &type) {
    inlet_prototypes_.push_back( InletPrototype(name, &Inlet::cast_method<R, T, Tmethod>, type) );
  }

  /** Declare an outlet. */
  void add_outlet(const char *name, const Value &type) {
    outlet_prototypes_.push_back( OutletPrototype(name, type) );
  }

  /** Build all inlets for an object from prototypes. */
  void make_inlets(Node *object);

  /** Build all inlets for an object from prototypes. */
  void make_outlets(Node *object);

  /** Build all methods for an object from prototypes. */
  void make_methods(Object *object) {
    std::list<MethodPrototype>::iterator it;
    std::list<MethodPrototype>::iterator end = method_prototypes_.end();

    for (it = method_prototypes_.begin(); it != end; it++) {
      object->adopt(new Method(object, *it));
    }
  }

private:

  std::list<InletPrototype>  inlet_prototypes_;   /**< Prototypes to create inlets. */
  std::list<OutletPrototype> outlet_prototypes_;  /**< Prototypes to create outlets. */
  std::list<MethodPrototype> method_prototypes_;  /**< Prototypes to create methods. */
};

// HELPER FOR FAST AND EASY ACCESSOR CREATION
#define ATTR_ACCESSOR(var, name) const Value name ## _accessor (const Value &val)    \
{ if (val.type_id() == var.type_id()) var = val;                                     \
  return var; }                                                                      \

// HELPERS TO AVOID TEMPLATE SYNTAX
#define CLASS(klass, info, options)  Class *c = planet.classes()->declare<klass>(#klass, info, options);
#define CLASS_NAMED(klass, name, info, options)  Class *c = planet.classes()->declare<klass>(name, info, options);
#define CLASS_METHOD(klass, method, info) c->add_class_method(#method, &klass::method, info);
#define METHOD(klass, method, type)         ADD_METHOD(klass, #method, method, type); \
                                            ADD_INLET(klass,  #method, method, type);
#define SUPER_METHOD(klass, super, method, type)  ADD_SUPER_METHOD(klass, super, method, type); \
                                            ADD_SUPER_INLET(klass, super, method, type);
#define ACCESSOR(klass, method, type)       ADD_METHOD(klass, #method, method ## _accessor, type);
#define ADD_METHOD(klass, name, method, type) c->add_method<klass, &klass::method>(name, type);
#define ADD_INLET(klass,  name, method, type) c->add_inlet<klass, &klass::method>(name, type);
#define ADD_SUPER_METHOD(klass, super, method, type) c->add_method<klass, super, &super::method>(#method, type);
#define ADD_SUPER_INLET(klass,  super, method, type) c->add_inlet<klass,  super, &super::method>(#method, type);
#define INLET(klass,  method, type) c->add_inlet<klass, &klass::method>(#method, type);
#define OUTLET(klass, name,   type) c->add_outlet(#name, type);

} // rk

#endif // RUBYK_INCLUDE_RUBYK_CLASS_H_
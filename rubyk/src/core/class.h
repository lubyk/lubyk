#ifndef _CLASS_H_
#define _CLASS_H_
#include "node.h"
#include "planet.h"

#include <list>


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
    method_Prototypes.push_back( MethodPrototype(name, &Method::cast_method<T, Tmethod>, type) );
  }
  
  /** Declare an inlet. */
  template <class T, void(T::*Tmethod)(const Value &val)>
  void add_inlet(const char *name, const Value &type) { 
    inlet_prototypes_.push_back( InletPrototype(name, &Inlet::cast_method<T, Tmethod>, type) );
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
    std::list<MethodPrototype>::iterator end = method_Prototypes.end();
    
    for (it = method_Prototypes.begin(); it != end; it++) {
      object->adopt(new Method(object, *it));
    }
  }
  
private:
  
  std::list<InletPrototype>  inlet_prototypes_;   /**< Prototypes to create inlets. */
  std::list<OutletPrototype> outlet_prototypes_;  /**< Prototypes to create outlets. */
  std::list<MethodPrototype> method_Prototypes;  /**< Prototypes to create methods. */
};

// HELPER FOR FAST AND EASY ACCESSOR CREATION
#define ATTR_ACCESSOR(var, name) const Value name ## _accessor (const Value &val)    \
{ if (val.type_id() == var.type_id()) var = val;                                     \
  return var; }                                                                      \

// HELPERS TO AVOID TEMPLATE SYNTAX
#define CLASS(klass, info, options)  Class * c = planet.classes()->declare<klass>(#klass, info, options);
#define CLASS_METHOD(klass, method, info) c->add_class_method(#method, &klass::method, info);
#define ACCESSOR(klass, method, type)       ADD_METHOD(klass, #method, method ## _accessor, type);
#define ADD_METHOD(klass, name, method, type)       c->add_method<klass, &klass::method>(name, type);
#define INLET(klass,  method, type) c->add_inlet<klass, &klass::method>(#method, type);
#define OUTLET(klass, name,   type) c->add_outlet(#name, type);
#endif // _CLASS_H_
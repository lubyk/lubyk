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
  
  Class(const char* pName, const char* info) : Object(pName) {
    set_info(info);
  }
  
  virtual ~Class() {}
  
  /** Add a new class method to the class. */
  void add_class_method(const char *name, class_method_t method, TypeTagID type_tag_id, const char *info) {
    ClassMethod * m = adopt(new ClassMethod(name, method, type_tag_id));
    m->set_info(info);
  }
  
  /** Declare a method. */
  template <class T, const Value(T::*Tmethod)(const Value &val)>
  void add_method(const char *name, TypeTagID type_tag_id, const char *info) { 
    method_Prototypes.push_back( MethodPrototype(name, &Method::cast_method<T, Tmethod>, type_tag_id, info) );
  }
  
  /** Declare an inlet. */
  template <class T, void(T::*Tmethod)(const Value &val)>
  void add_inlet(const char *name, const Value& type, const char *info) { 
    inlet_prototypes_.push_back( InletPrototype(name, &Inlet::cast_method<T, Tmethod>, type, info) );
  }
  
  /** Declare an outlet. */
  void add_outlet(const char *name, TypeTagID type_tag_id, const char *info) {
    outlet_prototypes_.push_back( OutletPrototype(name, type_tag_id, info) );    
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
#define ATTR_ACCESSOR(var, name) const Value name ## _ (const Value &val)    \
{ var = val;                                                        \
  return var; }                                                     \

// HELPERS TO AVOID TEMPLATE SYNTAX
#define CLASS(klass, info, options)  Class * c = planet.classes()->declare<klass>(#klass, info, options);
#define CLASS_METHOD(klass, method, info) c->add_class_method(#method, &klass::method, info);
#define ACCESSOR(klass, method, info)       ACCESSOR_NAMED(klass, #method, method ## _, info);
#define ACCESSOR_NAMED(klass, name, method, info)       c->add_method<klass, &klass::method>(name, info);
#define INLET(klass,  method, type, info) c->add_inlet<klass, &klass::method>(#method, type, info);
#define OUTLET(klass, name,   type, info) c->add_outlet(type, #name, info);
#endif // _CLASS_H_
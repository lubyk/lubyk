#ifndef _CLASS_H_
#define _CLASS_H_
#include "node.h"
#include "root.h"
#include "method.h"
#include <list>


/** This is a helper to prepare prototypes to:
  * 1. create nodes
  * 2. create node methods
  * 3. create node inlets / outlets
  */
class Class : public Object
{
public:
  Class(const char* pName, const char* pInfo) : Object(pName) 
  {
    set_info(pInfo);
  }
  
  virtual ~Class() {}
  
  /** Class signature. */
  virtual uint type()
  {
    return H("Class");
  }
  
  /** Add a new class method to the class. */
  void add_class_method(const char * pName, class_method_t pMethod, const char * pInfo)
  {
    ClassMethod * m = adopt(new ClassMethod(pName, pMethod));
    m->set_info(pInfo);
  }
  
  /** Declare a method. */
  template <class T, const Value(T::*Tmethod)(const Value& val)>
  void add_method (const char * pName, const char * pInfo)
  { 
    mMethodPrototypes.push_back( MethodPrototype(pName, &Method::cast_method<T, Tmethod>, pInfo) );
  }
  
  /** Declare an inlet. */
  template <class T, void(T::*Tmethod)(const Value& val)>
  void add_inlet (const char * pName, uint pType, const char * pInfo)
  { 
    mInletPrototypes.push_back( InletPrototype(pName, pType, &Inlet::cast_method<T, Tmethod>, pInfo) );
  }
  
  /** Declare an outlet. */
  void add_outlet(const char * pName, uint pType, const char * pInfo)
  {
    mOutletPrototypes.push_back( OutletPrototype(pName, pType, pInfo) );    
  }
  
  /** Build all inlets for an object from prototypes. */
  void make_inlets(Node * pObj)
  {
    std::list<InletPrototype>::iterator it;
    std::list<InletPrototype>::iterator end = mInletPrototypes.end();
    Object * inlets = pObj->adopt(new Object("inlets"));
    
    for (it = mInletPrototypes.begin(); it != end; it++) {
      inlets->adopt(new Inlet(pObj, *it));
    }
  }
  
  /** Build all inlets for an object from prototypes. */
  void make_outlets(Node * pObj)
  {
    std::list<OutletPrototype>::iterator it;
    std::list<OutletPrototype>::iterator end = mOutletPrototypes.end();
    Object * outlets = pObj->adopt(new Object("outlets"));
    Outlet * o;
    
    for (it = mOutletPrototypes.begin(); it != end; it++) {
      o = outlets->adopt(new Outlet(pObj, *it));
    }
  }
  
  /** Build all methods for an object from prototypes. */
  void make_methods(Node * pObj)
  {
    std::list<MethodPrototype>::iterator it;
    std::list<MethodPrototype>::iterator end = mMethodPrototypes.end();
    
    for (it = mMethodPrototypes.begin(); it != end; it++) {
      pObj->adopt(new Method(pObj, *it));
    }
  }
  
private:
  
  std::list<InletPrototype>  mInletPrototypes;   /**< Prototypes to create inlets. */
  std::list<OutletPrototype> mOutletPrototypes;  /**< Prototypes to create outlets. */
  std::list<MethodPrototype> mMethodPrototypes;  /**< Prototypes to create methods. */
};

// HELPER FOR FAST AND EASY ACCESSOR CREATION
#define ATTR_ACCESSOR(var, name) const Value name ## _ (const Value& val)    \
{ var = val;                                                        \
  return var; }                                                     \

// HELPERS TO AVOID TEMPLATE SYNTAX
#define CLASS(klass, info)                Class * c = root.classes()->declare<klass>(#klass, info);
#define CLASS_METHOD(klass, method, info) c->add_class_method(#method, &klass::method, info);
#define ACCESSOR(klass, method, info)       ACCESSOR_NAMED(klass, #method, method ## _, info);
#define ACCESSOR_NAMED(klass, name, method, info)       c->add_method<klass, &klass::method>(name, info);
#define INLET(klass,  method, types, info) c->add_inlet<klass, &klass::method>(#method, types, info);
#define OUTLET(klass, name,   types, info) c->add_outlet(#name, types, info);
#endif // _CLASS_H_
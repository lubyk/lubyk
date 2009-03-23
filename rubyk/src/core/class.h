#ifndef _CLASS_H_
#define _CLASS_H_
#include "node.h"
#include "planet.h"
#include "method.h"
#include <list>


/** This is a helper to prepare prototypes to:
  * 1. create nodes
  * 2. create node methods
  * 3. create node inlets / outlets
  */
class Class : public RObject
{
public:
  Class(const char* pName, const char* pInfo) : RObject(pName) 
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
  void add_class_method(const char * pTagTypeStr, const char * pName, class_method_t pMethod, const char * pInfo)
  {
    ClassMethod * m = adopt(new ClassMethod(pTagTypeStr, pName, pMethod));
    m->set_info(pInfo);
  }
  
  /** Declare a method. */
  template <class T, const Value(T::*Tmethod)(const Value val)>
  void add_method (const char * pTagTypeStr, const char * pName, const char * pInfo)
  { 
    mMethodPrototypes.push_back( MethodPrototype(pTagTypeStr, pName, &Method::cast_method<T, Tmethod>, pInfo) );
  }
  
  /** Declare an inlet. */
  template <class T, void(T::*Tmethod)(const Value val)>
  void add_inlet (const char * pTagTypeStr, const char * pName, const char * pInfo)
  { 
    mInletPrototypes.push_back( InletPrototype(pTagTypeStr, pName, &Inlet::cast_method<T, Tmethod>, pInfo) );
  }
  
  /** Declare an outlet. */
  void add_outlet(const char * pTagTypeStr, const char * pName, const char * pInfo)
  {
    mOutletPrototypes.push_back( OutletPrototype(pTagTypeStr, pName, pInfo) );    
  }
  
  /** Build all inlets for an object from prototypes. */
  void make_inlets(Node * pObj);
  
  /** Build all inlets for an object from prototypes. */
  void make_outlets(Node * pObj);
  
  /** Build all methods for an object from prototypes. */
  void make_methods(oscit::Object * pObj)
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
#define ATTR_ACCESSOR(var, name) const Value name ## _ (const Value val)    \
{ var = val;                                                        \
  return var; }                                                     \

// HELPERS TO AVOID TEMPLATE SYNTAX
#define CLASS(klass, info)                Class * c = planet.classes()->declare<klass>(#klass, info);
#define CLASS_METHOD(klass, method, info) c->add_class_method(#method, &klass::method, info);
#define ACCESSOR(klass, method, info)       ACCESSOR_NAMED(klass, #method, method ## _, info);
#define ACCESSOR_NAMED(klass, name, method, info)       c->add_method<klass, &klass::method>(name, info);
#define INLET(klass,  method, type, info) c->add_inlet<klass, &klass::method>(type, #method, info);
#define OUTLET(klass, name,   type, info) c->add_outlet(type, #name, info);
#endif // _CLASS_H_
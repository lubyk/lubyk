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
  
  virtual ~Class()
  {
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
  
  /** Declare an inlet, with an accessor method. */
  template <class T, void(T::*Tmethod)(const Value& val)>
  void add_inlet (const char * pName, value_t pAcceptTypes, const char * pInfo)
  { 
    mInletPrototypes.push_back( InletPrototype(pName, pAcceptTypes, &Inlet::cast_method<T, Tmethod>, pInfo) );
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
  
  std::list<InletPrototype>  mInletPrototypes;  /**< Prototypes to create inlets. */
  std::list<MethodPrototype> mMethodPrototypes;  /**< Prototypes to create methods. */
};

// HELPERS TO AVOID TEMPLATE SYNTAX
#define CLASS(klass, info)                Class * c = root.classes()->declare<klass>(#klass, info);
#define CLASS_METHOD(klass, method, info) c->add_class_method(#method, &klass::method, info);
#define METHOD(klass, method, info)       c->add_method<klass, &klass::method>(#method, info);
#endif // _CLASS_H_
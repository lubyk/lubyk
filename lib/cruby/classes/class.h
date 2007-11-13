#ifndef _CLASS_H_
#define _CLASS_H_
#include "node.h"
#include <iostream>

#define _TEST_OBJECTS_

struct lua_State;

/** Pointer to a member method that can be called from the command line with "obj.method(Params)" */
typedef void (*member_method_t)(void * pReceiver, const Params& p);

/** Pointer to a function to create nodes. */
typedef Node * (*create_function_t)(Class * pClass, const std::string& pName, Rubyk * pServer, const Params& p, std::ostream * pOutput);

/** Pointer to an inlet method that can be called from the command line with "obj.method(Params)" */
typedef void (*outlet_method_t)(void * pReceiver, Signal& sig);

/** Pointer to a method callable from lua. */
typedef int (*method_for_lua_t)(lua_State * L);

class Class
{
public:
  Class (const char* pName, create_function_t pFunction) : mName(pName), mCreateFunction(pFunction), mMethods(10), mMethodsForLua(10), mClassMethods(10) {}
  
  /** Execute a class method. Example: Midi.outputs */
  void execute_method (const std::string& pMethod, const Params& p, std::ostream * pOutput) ;

  /** Declare a class method. */
  void add_class_method(const char* pName, class_method_t pMethod)
  {
    mClassMethods.set(pName, pMethod);
  }
  
  /** Declare a member method. With parameters. */
  template <class T, void(T::*Tmethod)(const Params& pParam)>
  void add_method (const char* pName)
  {
    mMethods.set(std::string(pName), &cast_member_method<T, Tmethod>);
  }
  
  /** Declare a member method from a superclass. With parameters. */
  template <class T, class S, void(S::*Tmethod)(const Params& pParam)>
  void add_super_method (const char* pName)
  {
    mMethods.set(std::string(pName), &cast_super_member_method<T, S, Tmethod>);
  }
  
  /** Declare a member method. Parameters ignored. */
  template <class T, void(T::*Tmethod)()>
  void add_method (const char* pName)
  {
    mMethods.set(std::string(pName), &cast_member_method<T, Tmethod>);
  }
  
  /** Declare a method for lua. */
  void add_method_for_lua (const char* pName, method_for_lua_t pMethod)
  {
    mMethodsForLua.set(std::string(pName), pMethod);
  }
  
  
  /** Declare an inlet, with an accessor method. */
  template <class T, void(T::*Tmethod)(const Signal& sig)>
  void add_inlet (const char* pName)
  {  
    mInlets.push_back( &cast_inlet_method<T, Tmethod>);
    mMethods.set(std::string(pName), &cast_inlet_accessor<T, Tmethod>);
  }
  
  /** Declare an outlet. Outlets do not have callbacks. */
  void add_outlet (const char* pName)
  {
    mOutlets.push_back( std::string(pName) );
  }
  
  ////// class methods ///////
  
  /** Get a class from the class name. Returns false if the class could not be found nor loaded. */
  static bool get (Class ** pClass, const std::string& pClassName);
  
  static Class * find (const char * pClassName)
  {
    Class * res;
    if (get(&res, std::string(pClassName))) {
      return res;
    } else {
      return NULL; //fixme, this is bad
    }
  }
  
  static Node * create (Rubyk * pServer, const char * pClassName, const std::string& p, std::ostream * pOutput)
  { return create(pServer, std::string(""), std::string(pClassName), Params(p), pOutput); }
  
  
  static Node * create (Rubyk * pServer, const char * pName, const char * pClassName, const std::string& p, std::ostream * pOutput)
  { return create(pServer, std::string(pName), std::string(pClassName), Params(p), pOutput); }

  static Node * create (Rubyk * pServer, const char * pName, const char * pClassName, const char * p, std::ostream * pOutput)
  { return create(pServer, std::string(pName), std::string(pClassName), Params(p), pOutput); }

  static Node * create (Rubyk * pServer, const std::string& pName, const std::string& pClassName, const char * p, std::ostream * pOutput)
  { return create(pServer, pName, pClassName, Params(p), pOutput); }

  static Node * create (Rubyk * pServer, const std::string& pName, const std::string& pClassName, const std::string& p, std::ostream * pOutput)
  { return create(pServer, pName, pClassName, Params(p), pOutput); }

  static Node * create (Rubyk * pServer, const char * pName, const char * pClassName, const Params& p, std::ostream * pOutput)
  { return create(pServer, std::string(pName), std::string(pClassName), p, pOutput); }

  static Node * create (Rubyk * pServer, const std::string& pName, const std::string& pClassName, const Params& p, std::ostream * pOutput);

  /** Load an object stored in a dynamic library. */
  static bool load(const char * file, const char * init_name);

  template<class T>
  static Class * declare(const char* name)
  {
    Class * klass;
    if (sClasses.get(&klass, std::string(name)))
      delete klass; // the reference will be lost
    
    klass = new Class(name, &cast_create<T>);
    sClasses.set(std::string(name), klass);
    klass->add_method<Node, &Node::do_bang>("bang");
    return klass;
  }

  static void set_lib_path(const char* pPath)
  { sObjectsPath = pPath; }

  static void set_lib_path(const std::string& pPath)
  { sObjectsPath = pPath; }

  const std::string& name() { return mName; }
  
  bool get_member_method(member_method_t * pMethod, const std::string& pMethodName)
  { return mMethods.get(pMethod, pMethodName); }
  
  const Hash<std::string, method_for_lua_t> * methodsForLua ()
  { return &mMethodsForLua;}
  
private:
  
  inline Node * operator() (const std::string& pName, Rubyk * pServer, const Params& p, std::ostream * pOutput);
  
  inline void Class::make_slots (Node * node)
  {
    int i,sz;
    
    sz = mInlets.size();
    for(i = 0; i < sz; i++) {
      node->add_inlet(mInlets[i]);
    }

    sz = mOutlets.size();
    for(i = 0; i < sz; i++) {
      node->add_outlet(mOutlets[i]);
    }
  }
  
  static Hash<std::string, Class*> sClasses; /**< Contains a dictionary of class names and Class objects. For example, 'metro' => function to create a Metro. */
  
  static std::string sObjectsPath; /**< Where to load the librairies (objects). */
  
  /** This function is used to create an instance of class 'T'. If the instance could not be
    * properly initialized, this function returns NULL. */
  template<class T>
  static Node * cast_create(Class * pClass, const std::string& pName, Rubyk * pServer, const Params& p, std::ostream * pOutput)
  {
    T * obj = new T;
    obj->set_class(pClass);
    obj->set_name(pName);
    obj->set_server(pServer);
    obj->set_output(pOutput);
    pClass->make_slots(obj);
    obj->set_is_ok( obj->init(p) ); // if init returns false, the node goes into 'broken' mode.
    return (Node*)obj;
  }
  
  /** Return a function pointer to a member method. */
  template <class T, void(T::*Tmethod)(const Params& p)>
  static void cast_member_method(void * receiver, const Params& p)
  {
    (((T*)receiver)->*Tmethod)(p);
  }
  
  /** Return a function pointer to a superclass member method. */
  template <class T, class S, void(S::*Tmethod)(const Params& p)>
  static void cast_super_member_method(void * receiver, const Params& p)
  {
    (((T*)receiver)->*Tmethod)(p);
  }
  
  /** Return a function pointer to a member method without parameters. */
  template <class T, void(T::*Tmethod)()>
  static void cast_member_method(void * receiver, const Params& p)
  {
    (((T*)receiver)->*Tmethod)();
  }
  
  
  /** Transform an inlet callback into a 'Params' based accessor. */
  template <class T, void(T::*Tmethod)(const Signal& sig)>
  static void cast_inlet_accessor (void * receiver, const Params& p)
  {
    Signal sig;
    double value;
    if (p.get(&value)) {
      sig.set(value);
    } else {
      sig.set_bang();
    }
    (((T*)receiver)->*Tmethod)(sig);
  }
  
  /** Transform an outlet callback into a 'Params' based accessor. */
  template <class T, void(T::*Tmethod)(Signal& sig)>
  static void cast_outlet_accessor (void * receiver, const Params& p)
  {
    Signal sig;
    (((T*)receiver)->*Tmethod)(sig);
    ((Node*)receiver)->output() << sig << std::endl;
  }
  
  /** Create a callback for an outlet. */
  template <class T, void(T::*Tmethod)(Signal& sig)>
  static void cast_outlet_method (void * receiver, Signal& sig)
  {
    (((T*)receiver)->*Tmethod)(sig);
  }
  
  /** Create a callback for an inlet. */
  template <class T, void(T::*Tmethod)(const Signal& sig)>
  static void cast_inlet_method (void * receiver, const Signal& sig)
  {
    (((T*)receiver)->*Tmethod)(sig);
  }
  
  
  /* class info */
  std::string                          mName;           /**< Class name. */
  create_function_t                    mCreateFunction; /**< Function to create a new instance. */
  Hash<std::string, member_method_t>   mMethods;        /**< Member methods. */
  Hash<std::string, class_method_t>    mClassMethods;   /**< Class methods. */
  Hash<std::string, method_for_lua_t>  mMethodsForLua;  /**< Class methods. */
  
  
  
  std::vector<inlet_method_t>         mInlets;         /**< Inlet prototypes.  */
  std::vector<std::string>            mOutlets;        /**< Outlet prototypes (they just contain a name). */
};

// HELPERS TO AVOID TEMPLATE SYNTAX
#define CLASS(klass)         {Class::declare<klass>(#klass);}
#define INLET(klass,method)  {Class::find(#klass)->add_inlet<klass, &klass::method>(#method);}
#define OUTLET(klass,method) {Class::find(#klass)->add_outlet(#method);}
#define METHOD(klass,method) {Class::find(#klass)->add_method<klass, &klass::method>(#method);}
#define CLASS_METHOD(klass,method) {Class::find(#klass)->add_class_method(#method, &klass::method);}

#define SUPER_METHOD(klass,super,method) {Class::find(#klass)->add_super_method<klass, super, &super::method>(#method);}
#endif
#ifndef _CLASS_H_
#define _CLASS_H_
#include "node.h"
#include <iostream>

/** Pointer to a function to create nodes. */
typedef Node * (*create_function_t)(Class * pClass, Rubyk * pServer, const Params& p, std::ostream * pOutput);

/** Pointer to a member method that can be called from the command line with "obj.method(Params)" */
typedef void (*member_method_t)(void * pReceiver, const Params& p);

/** Pointer to a class method that can be called from the command line with "Value.method(Params)" */
typedef void (*class_method_t)(std::ostream * pOutput, const Params& p);

class Class
{
public:
  Class (const char* pName, create_function_t pFunction) : mName(pName), mCreateFunction(pFunction), mMethods(10), mClassMethods(10) {}
  
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
  
  /** Declare a member method. Parameters ignored. */
  template <class T, void(T::*Tmethod)()>
  void add_method (const char* pName)
  {
    mMethods.set(std::string(pName), &cast_member_method<T, Tmethod>);
  }

  ////// class methods ///////
  
  /** Get a class from the class name. Returns false if the class could not be found nor loaded. */
  static bool get (Class ** pClass, const std::string& pClassName);
  
  static Node * create (Rubyk * pServer, const char * pClassName, const std::string& p, std::ostream * pOutput)
  { return create(pServer, std::string(pClassName), Params(p), pOutput); }

  static Node * create (Rubyk * pServer, const char * pClassName, const char * p, std::ostream * pOutput)
  { return create(pServer, std::string(pClassName), Params(p), pOutput); }

  static Node * create (Rubyk * pServer, const std::string& pClassName, const char * p, std::ostream * pOutput)
  { return create(pServer, pClassName, Params(p), pOutput); }

  static Node * create (Rubyk * pServer, const std::string& pClassName, const std::string& p, std::ostream * pOutput)
  { return create(pServer, pClassName, Params(p), pOutput); }

  static Node * create (Rubyk * pServer, const char * pClassName, const Params p, std::ostream * pOutput)
  { return create(pServer, std::string(pClassName), Params(p), pOutput); }

  static Node * create (Rubyk * pServer, const std::string& pClassName, const Params& p, std::ostream * pOutput);

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
    return klass;
  }

  static void set_lib_path(const char* pPath)
  { sObjectsPath = pPath; }

  static void set_lib_path(const std::string& pPath)
  { sObjectsPath = pPath; }

  const std::string& name() { return mName; }
  
private:
  friend class Node;
  
  inline Node * operator() (Rubyk * pServer, const Params& p, std::ostream * pOutput);
  
  static Hash<std::string, Class*> sClasses; /**< Contains a dictionary of class names and Class objects. For example, 'metro' => function to create a Metro. */
  
  static std::string sObjectsPath; /**< Where to load the librairies (objects). */
  
  /** This function is used to create an instance of class 'T'. If the instance could not be
    * properly initialized, this function returns NULL. */
  template<class T>
  static Node * cast_create(Class * pClass, Rubyk * pServer, const Params& p, std::ostream * pOutput)
  {
    T * obj = new T;
    obj->set_class(pClass);
    obj->set_server(pServer);
    obj->set_output(pOutput);
    obj->set_is_ok( obj->init(p) ); // if init returns false, the node goes into 'broken' mode.
    return (Node*)obj;
  }
  
  /** Return a function pointer to a member method. */
  template <class T, void(T::*Tmethod)(const Params& p)>
  static void cast_member_method(void * receiver, const Params& p)
  {
    (((T*)receiver)->*Tmethod)(p);
  }
  
  /** Return a function pointer to a member method. */
  template <class T, void(T::*Tmethod)()>
  static void cast_member_method(void * receiver, const Params& p)
  {
    (((T*)receiver)->*Tmethod)();
  }
  /* class info */
  std::string                         mName;           /**< Class name. */
  create_function_t                   mCreateFunction; /**< Function to create a new instance. */
  Hash<std::string, member_method_t>  mMethods;        /**< Member methods. */
  Hash<std::string, class_method_t>   mClassMethods;   /**< Class methods. */
};

#endif
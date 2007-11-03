#ifndef _CLASS_H_
#define _CLASS_H_
#include "node.h"
#include <iostream>

/** Pointer to a function to create nodes. */
typedef Node * (*create_function_t)(Class * pClass, Rubyk * pServer, const Params& pParams);

/** Pointer to a member method that can be called from the command line with "obj.method(Params)" */
typedef void (*member_method_t)(void * pReceiver, const Params& pParams);

/** Pointer to a class method that can be called from the command line with "Value.method(Params)" */
typedef void (*class_method_t)(std::ostream * pOutput, const Params& pParam);

class Class
{
public:
  Class (const char* pName, create_function_t pFunction) : mName(pName), mCreateFunction(pFunction), mMethods(10), mClassMethods(10) {}

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
  
  static Node * create (Rubyk * pServer, const char * pKey, const std::string& pParams)
  { return create(pServer, std::string(pKey), Params(pParams)); }

  static Node * create (Rubyk * pServer, const char * pKey, const char * pParams)
  { return create(pServer, std::string(pKey), Params(pParams)); }

  static Node * create (Rubyk * pServer, const std::string& pKey, const char * pParams)
  { return create(pServer, pKey, Params(pParams)); }

  static Node * create (Rubyk * pServer, const std::string& pKey, const std::string& pParams)
  { return create(pServer, pKey, Params(pParams)); }

  static Node * create (Rubyk * pServer, const char * pKey, const Params pParams)
  { return create(pServer, std::string(pKey), Params(pParams)); }

  static Node * create (Rubyk * pServer, const std::string& pKey, const Params& pParams);

  /** Load an object stored in a dynamic library. */
  static bool load(const char * file, const char * init_name);

  template<class T>
  static Class * declare(const char* name)
  {
    Class ** klass_ptr = sClasses.get(std::string(name));
    Class * klass;
    if (klass_ptr)
      delete *klass_ptr; // the reference will be lost
    
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
  
  inline Node * operator() (Rubyk * pServer, const Params& pParams);
  
  static Hash<std::string, Class*> sClasses; /**< Contains a dictionary of class names and Class objects. For example, 'metro' => function to create a Metro. */
  
  static std::string sObjectsPath; /**< Where to load the librairies (objects). */
  
  /** This function is used to create an instance of class 'T'. If the instance could not be
    * properly initialized, this function returns NULL. */
  template<class T>
  static Node * cast_create(Class * pClass, Rubyk * pServer, const Params& pParams)
  {
    T * obj = new T;
    obj->set_class(pClass);
    obj->set_server(pServer);
    obj->set_is_ok( obj->init(pParams) ); // if init returns false, the node goes into 'broken' mode.
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
#ifndef _METHOD_H_
#define _METHOD_H_
#include "object.h"

class Root;

/** Function pointer to trigger a member method. */
typedef const Value (*member_method_t)(void * pReceiver, const Value& p);

/** Function pointer to trigger a class method. */
typedef const Value (*class_method_t )(Root& pRoot, const Value& p);

/** Object instance to trigger a class method. */
class ClassMethod : public Object
{
public:
  ClassMethod(const std::string &pName, class_method_t pMethod) : Object(pName), mMethod(pMethod) {}
  ClassMethod(const char * pName, class_method_t pMethod) : Object(pName), mMethod(pMethod) {}
  
  virtual const Value trigger (const Value& val)
  {
    return (*mMethod)(*mRoot, val);
  }
  
protected:
  class_method_t mMethod;   /**< Pointer on the class method. */ 
};


/** Prototype constructor for Method. */
struct MethodPrototype
{
  MethodPrototype(const char * pName, member_method_t pMethod, const char * pInfo) : mName(pName), mMethod(pMethod), mInfo(pInfo) {}
  const char *    mName;
  member_method_t mMethod;
  const char *    mInfo;
};

/** Object instance to trigger a member method (stores a reference to the receiver). */
class Method : public Object
{
public:
  Method(const std::string &pName, void * pReceiver, member_method_t pMethod) : Object(pName), mReceiver(pReceiver), mMethod(pMethod) {}
  
  Method(const char * pName, void * pReceiver, member_method_t pMethod) : Object(pName), mReceiver(pReceiver), mMethod(pMethod) {}
  
  /** Prototype based constructor. */
  Method (void * pReceiver, const MethodPrototype& pProto) : Object(pProto.mName), mReceiver(pReceiver), mMethod(pProto.mMethod)
  {
    set_info(pProto.mInfo);
  }
  
  virtual const Value trigger (const Value& val)
  {
    return (*mMethod)(mReceiver,val);
  }
  
protected:
  /** Make a pointer to a member method. */
  template<class T, const Value(T::*Tmethod)(const Value&)>
  static const Value cast_method (void * pReceiver, const Value& val)
  {
    return (((T*)pReceiver)->*Tmethod)(val);
  }

  void *          mReceiver; /**< Object containing the method. */
  member_method_t mMethod;   /**< Pointer on a cast of the member method. */ 
};

#endif
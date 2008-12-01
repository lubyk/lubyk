#ifndef _METHOD_H_
#define _METHOD_H_
#include "object.h"

/** Function pointer to trigger a member method. */
typedef const Value (*member_method_t)(void * pReceiver, const Value& p);

/** Function pointer to trigger a class method. */
typedef const Value (*class_method_t )(const Value& p);

/** Object instance to trigger a class method. */
class ClassMethod : public Object
{
public:
  ClassMethod(Object * pParent, const std::string &pName, class_method_t pMethod) : Object(pParent, pName), mMethod(pMethod) {}
  ClassMethod(Object&  pParent, const std::string &pName, class_method_t pMethod) : Object(pParent, pName), mMethod(pMethod) {}
  ClassMethod(Object * pParent, const char * pName, class_method_t pMethod) : Object(pParent, pName), mMethod(pMethod) {}
  ClassMethod(Object&  pParent, const char * pName, class_method_t pMethod) : Object(pParent, pName), mMethod(pMethod) {}
  
  virtual const Value trigger (const Value& val)
  {
    return (*mMethod)(val);
  }
  
private:
  class_method_t mMethod;   /**< Pointer on the class method. */ 
};

/** Object instance to trigger a member method (stores a reference to the receiver). */
/** FIXME: !!! should be destroyed with receiver !!!! */
class Method : public Object
{
public:
  Method(Object * pParent, const std::string &pName, void * pReceiver, member_method_t pMethod) : Object(pParent, pName), mReceiver(pReceiver), mMethod(pMethod) {}
  Method(Object&  pParent, const std::string &pName, void * pReceiver, member_method_t pMethod) : Object(pParent, pName), mReceiver(pReceiver), mMethod(pMethod) {}
  Method(Object * pParent, const char * pName, void * pReceiver, member_method_t pMethod) : Object(pParent, pName), mReceiver(pReceiver), mMethod(pMethod) {}
  Method(Object&  pParent, const char * pName, void * pReceiver, member_method_t pMethod) : Object(pParent, pName), mReceiver(pReceiver), mMethod(pMethod) {}
  
  virtual const Value trigger (const Value& val)
  {
    return (*mMethod)(mReceiver,val);
  }
  
private:
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
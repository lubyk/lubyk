#ifndef OSCIT_INCLUDE_OSCIT_METHOD_H_
#define OSCIT_INCLUDE_OSCIT_METHOD_H_
#include "oscit/object.h"

namespace oscit {

/** Function pointer to trigger a member method. */
typedef const Value (*member_method_t)(void *receiver, const Value &val);

/** Function pointer to trigger a class method. */
typedef const Value (*class_method_t )(Root *root, const Value &val);

/** Object instance to trigger a class method. */
class ClassMethod : public Object
{
 public: 
  /** Class signature. */
  TYPED("Object.ClassMethod")
  
  /** Create a new object that will call a class method when "triggered". */
  ClassMethod(const std::string &name, class_method_t method, const Value &type) :
      Object(name, type), class_method_(method) {}
  
  /** Create a new object that will call a class method when "triggered". */
  ClassMethod(const char *name, class_method_t method, const Value &type) :
      Object(name, type), class_method_(method) {}
  
  /** Trigger: call the class method. */
  virtual const Value trigger(const Value &val, const Location *origin) {
    return (*class_method_)(root_, val);
  }
  
 protected:
  class_method_t class_method_;   /**< Pointer on the class method. */ 
};


/** Prototype constructor for Method. */
struct MethodPrototype
{
  MethodPrototype(const char *name, member_method_t method, const Value &type) : 
      name_(name), member_method_(method), type_(type) {}
  const char *    name_;
  member_method_t member_method_;
  Value           type_;
};

/** Object instance to trigger a member method (stores a reference to the receiver). */
class Method : public Object
{
 public:
  TYPED("Object.Method")
  
  /** Create a new object that call a member method when "triggered". */
  Method(void *receiver, const char *name, member_method_t method, const Value &type) :
      Object(name, type), receiver_(receiver), member_method_(method) {}
  
  /** Create a new object that call a member method when "triggered". */
  Method(void *receiver, const std::string &name, member_method_t method, const Value &type) :
      Object(name, type), receiver_(receiver), member_method_(method) {}
  
  /** Prototype based constructor. */
  Method (void * receiver, const MethodPrototype &prototype) : Object(prototype.name_, prototype.type_),
      receiver_(receiver), member_method_(prototype.member_method_) {}
  
  /** Trigger: call the member method on the receiver. */
  virtual const Value trigger(const Value &val, const Location *origin)
  {
    return (*member_method_)(receiver_,val);
  }
  
  /** Make a pointer to a member method. */
  template<class T, const Value(T::*Tmethod)(const Value&)>
  static const Value cast_method(void *receiver, const Value &val)
  {
    return (((T*)receiver)->*Tmethod)(val);
  }
  
  /** Make a pointer to a member method. */
  template<class R, class T, const Value(T::*Tmethod)(const Value&)>
  static const Value cast_method(void *receiver, const Value &val)
  {
    return (((R*)receiver)->*Tmethod)(val);
  }
  
  /** Make a pointer to a member method without return values. */
  template<class T, void(T::*Tmethod)(const Value&)>
  static const Value cast_method(void *receiver, const Value &val)
  {
    (((T*)receiver)->*Tmethod)(val);
    return gNilValue;
  }
  
  /** Make a pointer to a member method without return values. */
  template<class R, class T, void(T::*Tmethod)(const Value&)>
  static const Value cast_method(void *receiver, const Value &val)
  {
    (((R*)receiver)->*Tmethod)(val);
    return gNilValue;
  }
 protected:
  void *          receiver_;       /**< Object containing the method. */
  member_method_t member_method_;  /**< Pointer on a cast of the member method. */ 
};

template<class T, const Value(T::*Tmethod)(const Value&)>
class TMethod : public Method
{
 public:
  TYPED("Object.TMethod.Method")
  
  /** Create a new object that call a member method when "triggered". */
  TMethod(void *receiver, const char *name, const Value &type) :
      Method(receiver, name, &cast_method<T, Tmethod>, type) {}
  
  /** Create a new object that call a member method when "triggered". */
  TMethod(void *receiver, const std::string &name, const Value &type) :
      Method(receiver, name, &cast_method<T, Tmethod>, type) {}
};

} // oscit

#endif // OSCIT_INCLUDE_OSCIT_METHOD_H_

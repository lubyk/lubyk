#ifndef _METHOD_H_
#define _METHOD_H_
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
  ClassMethod(const std::string &name, class_method_t method, TypeTagID type_tag_id, const char *info = DEFAULT_INFO) :
      Object(name, type_tag_id), class_method_(method) {
    set_info(info);
  }
  
  /** Create a new object that will call a class method when "triggered". */
  ClassMethod(const char *name, class_method_t method, TypeTagID type_tag_id, const char *info = DEFAULT_INFO) :
      Object(name, type_tag_id), class_method_(method) {
    set_info(info);
  }
  
  /** Trigger: call the class method. */
  virtual const Value trigger(const Value &val) {
    return (*class_method_)(root_, val);
  }
  
 protected:
  class_method_t class_method_;   /**< Pointer on the class method. */ 
};


/** Prototype constructor for Method. */
struct MethodPrototype
{
  MethodPrototype(const char *name, member_method_t method, TypeTagID type_tag_id, const char *info) : 
      name_(name), member_method_(method), type_tag_id_(type_tag_id), info_(info) {}
  const char *    name_;
  member_method_t member_method_;
  TypeTagID       type_tag_id_;
  const char *    info_;
};

/** Object instance to trigger a member method (stores a reference to the receiver). */
class Method : public Object
{
 public:
  TYPED("Object.Method")
  
  /** Create a new object that call a member method when "triggered". */
  Method(void *receiver, const char *name, member_method_t method, TypeTagID type_tag_id, const char *info = DEFAULT_INFO) :
      Object(name, type_tag_id), receiver_(receiver), member_method_(method) {
    set_info(info);
  }
  
  /** Create a new object that call a member method when "triggered". */
  Method(void *receiver, const std::string &name, member_method_t method, TypeTagID type_tag_id, const char *info = DEFAULT_INFO) :
      Object(name, type_tag_id), receiver_(receiver), member_method_(method) {
    set_info(info);
  }
  
  /** Prototype based constructor. */
  Method (void * receiver, const MethodPrototype &prototype) : Object(prototype.name_, prototype.type_tag_id_),
      receiver_(receiver), member_method_(prototype.member_method_) {
    set_info(prototype.info_);
  }
  
  /** Trigger: call the member method on the receiver. */
  virtual const Value trigger(const Value &val)
  {
    return (*member_method_)(receiver_,val);
  }
  
protected:
  /** Make a pointer to a member method. */
  template<class T, const Value(T::*Tmethod)(const Value&)>
  static const Value cast_method (void *receiver, const Value &val)
  {
    return (((T*)receiver)->*Tmethod)(val);
  }

  void *          receiver_;       /**< Object containing the method. */
  member_method_t member_method_;  /**< Pointer on a cast of the member method. */ 
};

template<class T, const Value(T::*Tmethod)(const Value&)>
class TMethod : public Method
{
 public:
  TYPED("Object.TMethod.Method")
  
  /** Create a new object that call a member method when "triggered". */
  TMethod(void *receiver, const char *name, TypeTagID type_tag_id, const char *info = DEFAULT_INFO) :
      Method(receiver, name, &cast_method<T, Tmethod>, type_tag_id, info) {}
  
  /** Create a new object that call a member method when "triggered". */
  TMethod(void *receiver, const std::string &name, TypeTagID type_tag_id, const char *info = DEFAULT_INFO) :
      Method(receiver, name, &cast_method<T, Tmethod>, type_tag_id, info) {}
};

} // oscit
#endif
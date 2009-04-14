#ifndef _OBJECT_H_
#define _OBJECT_H_
#include "oscit.h"

class Worker;

/** Object is the base object in Rubyk context. The object is just a wrapper around oscit::BaseObject
 *  with an extra "Worker" pointer. */
class Object : public BaseObject
{
 public:
   Object() : worker_(NULL) {}
   Object(const char *name) : BaseObject(name), worker_(NULL) {}
   Object(const std::string &name) : BaseObject(name), worker_(NULL) {}
   Object(TypeTagID type_tag_id) : BaseObject(type_tag_id), worker_(NULL) {}
   Object(const char *name, TypeTagID type_tag_id) : BaseObject(name, type_tag_id), worker_(NULL) {}
   Object(const std::string &name, TypeTagID type_tag_id) : BaseObject(name, type_tag_id), worker_(NULL) {}
   Object(Object *parent, const char *name) : BaseObject(parent, name), worker_(NULL) {}
   Object(Object *parent, const char *name, TypeTagID type_tag_id) : BaseObject(parent, name, type_tag_id), worker_(NULL) {}
   Object(Object *parent, const std::string &name, TypeTagID type_tag_id) : BaseObject(parent, name, type_tag_id), worker_(NULL) {}
   Object(Object &parent, const char *name) : BaseObject(parent, name), worker_(NULL) {}
   Object(Object &parent, const char *name, TypeTagID type_tag_id) : BaseObject(parent, name, type_tag_id), worker_(NULL) {}
   Object(Object &parent, const std::string &name, TypeTagID type_tag_id) : BaseObject(parent, name, type_tag_id), worker_(NULL) {}
  
  /** Class signature. */
  virtual uint class_type() {
    return H("Object");
  }
  
  /** Inherit the worker from the parent. */
  virtual void set_context(Mutex *context) {
    // FIXME: what do we do if context is NULL ?? (no more parent)
    // TODO: can we use a static_cast here ?
    context_ = context;
    worker_  = (Worker*)context_;
  }
  
 protected:
  Worker * worker_;  /**< Worker that will give life to object. */
};

#endif // _OBJECT_H_
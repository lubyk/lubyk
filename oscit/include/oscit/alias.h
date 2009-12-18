#ifndef OSCIT_INCLUDE_OSCIT_ALIAS_H_
#define OSCIT_INCLUDE_OSCIT_ALIAS_H_

#include "oscit/root.h"

namespace oscit {
/** This object triggers another object's trigger when called. It's used in Group to expose functionalities. */
class Alias : public Object
{
public:
  /** Class signature. */
  TYPED("Object.Alias")

  Alias() : original_(NULL) {}

  Alias(const char *name, Object *object) : Object(name, object->type()),
    original_(object) {
    original_->register_alias(this);
  }

  Alias(const std::string &name, Object *object) : Object(name, object->type()),
    original_(object) {
    original_->register_alias(this);
  }

  virtual ~Alias() {
    // We unregister to tell the object that it should not delete this alias on destruction.
    if (original_) original_->unregister_alias(this);
  }

  virtual const Value trigger(const Value &val) {
    return original_ ? original_->safe_trigger(val, context_) : gNilValue;
  }

  /** Used by original object during destruction (avoid ~Alias calling
   *  unregister_alias).
   */
  void unlink_original() {
    original_ = NULL;
  }

 protected:
  Object *original_; /**< Original object pointed to by the alias. */
};

} // oscit
#endif // OSCIT_INCLUDE_OSCIT_ALIAS_H_

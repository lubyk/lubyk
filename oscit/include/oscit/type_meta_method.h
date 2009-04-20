#ifndef _OSCIT_TYPE_META_METHOD_H_
#define _OSCIT_TYPE_META_METHOD_H_
#include "oscit/root.h"

namespace oscit {

class TypeMetaMethod : public Object
{
public:  
  /** Class signature. */
  TYPED("Object.TypeMetaMethod")
  
  TypeMetaMethod(const char *name) : Object(name, TextIO("url", "Return type and information on the given url.")) {}

  virtual const Value trigger(const Value &url) {
    Value error;
    Object * target = root_->find_or_build_object_at(url.c_str(), &error);
    
    if (target == NULL) {
      return error;
    } else {
      Value type = target->type();
      
      if (type.is_string()) {
        // meta type is string = just information (not callable)
        return type;
      }
      
      if (!type.is_list()) {
        // make sure type is a ListValue
        return ErrorValue(INTERNAL_SERVER_ERROR, "Invalid meta type. Should be a list (found '").append(type.type_tag()).append("').");
      }
      
      if (!type[0].is_any() && !type[0].is_nil()) {
        // get current value
        Value current = target->trigger(gNilValue);
      
        // make sure current value type is compatible with type
        if (current.type_id() != type[0].type_id()) {
          return ErrorValue(INTERNAL_SERVER_ERROR, "Current value type not matching meta type (expected '").append(
              type[0].type_tag()).append(
              "' found '").append(current.type_tag()).append("').");
        }
      
        type.set_value_at(0, current);
      }
      return type;
    }
  }
};

#endif // _OSCIT_TYPE_META_METHOD_H_

} // oscit
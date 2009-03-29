#ifndef _TYPE_META_METHOD_H_
#define _TYPE_META_METHOD_H_
#include "oscit/root.h"

namespace oscit {

class TypeMetaMethod : public Object
{
public:
  TypeMetaMethod(const char * name) : Object(name, H("s")) {}

  virtual const Value trigger (const Value &url) {
    Object * target = root_->find_or_build_object_at(url.s);
    
    if (target != NULL) {
      Value type = target->type();
      if (!type.is_nil()) {
        // make sure type is a ListValue
        if (!type.is_list()) return ErrorValue(INTERNAL_SERVER_ERROR, std::string("Invalid meta type. Should be a list (found '").append(type.type_tag()).append("')."));
        // get current value
        Value current = target->trigger(gNilValue);
        // make sure current value type is compatible with type
        if (current.type_tag_id() != type[0].type_tag_id()) {
          return ErrorValue(INTERNAL_SERVER_ERROR, 
              std::string("Current value type not matching meta type (expected '").append(
              type[0].type_tag()).append(
              "' found '").append(current.type_tag()).append("').")
              );
        }
        type.set_value_at(0, current);
        return type;
      } else {
        // meta type is nil = no type
        return gNilValue;
      }
    }
    return ErrorValue(NOT_FOUND_ERROR, url.s);
  }
};

#endif // _TYPE_META_METHOD_H_

} // oscit
#ifndef OSCIT_INCLUDE_OSCIT_TYPE_META_METHOD_H_
#define OSCIT_INCLUDE_OSCIT_TYPE_META_METHOD_H_
#include "oscit/root.h"

namespace oscit {

class TypeMetaMethod : public Object
{
public:  
  /** Class signature. */
  TYPED("Object.TypeMetaMethod")
  
  TypeMetaMethod(const char *name)        : Object(name, StringIO("path", "Return type and information on the given path.")) {}
  TypeMetaMethod(const std::string &name) : Object(name, StringIO("path", "Return type and information on the given path.")) {}

  virtual const Value trigger(const Value &path) {
    if (!path.is_string()) return gNilValue;
    Value error;
    Object * target = root_->find_or_build_object_at(path.c_str(), &error);
    
    Value reply = path;
    
    reply.push_back(target ? target->type_with_current_value() : error);
    return reply;
  }
};

} // oscit

#endif // OSCIT_INCLUDE_OSCIT_TYPE_META_METHOD_H_

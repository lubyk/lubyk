#ifndef OSCIT_INCLUDE_OSCIT_LIST_META_METHOD_H_
#define OSCIT_INCLUDE_OSCIT_LIST_META_METHOD_H_
#include "oscit/root.h"

namespace oscit {

class ListMetaMethod : public Object
{
public:
  /** Class signature. */
  TYPED("Object.ListMetaMethod")

  ListMetaMethod(const char *name)        : Object(name, StringIO("path", "List all children under the given path.")) {}
  ListMetaMethod(const std::string &name) : Object(name, StringIO("path", "List all children under the given path.")) {}

  virtual const Value trigger(const Value &val) {
    if (!val.is_string()) return gNilValue;

    Value error;
    Object * target = root_->find_or_build_object_at(val.c_str(), &error);

    Value reply = val;
    reply.push_back(target ? target->list() : error);
    return reply;
  }
};

} // oscit

#endif // OSCIT_INCLUDE_OSCIT_LIST_META_METHOD_H_

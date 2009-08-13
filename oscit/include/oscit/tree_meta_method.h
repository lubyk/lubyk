#ifndef OSCIT_INCLUDE_OSCIT_TREE_META_METHOD_H_
#define OSCIT_INCLUDE_OSCIT_TREE_META_METHOD_H_
#include "oscit/root.h"

namespace oscit {

class TreeMetaMethod : public Object
{
public:  
  /** Class signature. */
  TYPED("Object.TreeMetaMethod")
  
  TreeMetaMethod(const char *name) : Object(name, StringIO("path", "Returns all children and sub-children under the given path")) {}

  virtual const Value trigger(const Value &path) {
    if (!path.is_string()) return gNilValue;
    ListValue tmp;
    Object * target = root_->find_or_build_object_at(path.c_str(), &tmp);
    
    Value reply = path;
    if (target) {
      target->tree(target->url().length() + 1, &tmp);
    }
    reply.push_back(tmp);
    return reply;
  }
};

} // oscit

#endif // OSCIT_INCLUDE_OSCIT_TREE_META_METHOD_H_

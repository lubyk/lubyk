#ifndef _OSCIT_TREE_META_METHOD_H_
#define _OSCIT_TREE_META_METHOD_H_
#include "oscit/root.h"

namespace oscit {

class TreeMetaMethod : public Object
{
public:  
  /** Class signature. */
  TYPED("Object.TreeMetaMethod")
  
  TreeMetaMethod(const char *name) : Object(name, TextIO("url", "Returns all children and sub-children under the given url")) {}

  virtual const Value trigger(const Value &url) {
    Value res;
    Object * target = root_->find_or_build_object_at(url.c_str(), &res);
    if (!target) return res;
    target->tree(target->url().length() + 1, &res);
    return res;
  }
};

#endif // _OSCIT_TREE_META_METHOD_H_

} // oscit
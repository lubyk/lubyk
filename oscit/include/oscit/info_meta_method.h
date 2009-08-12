#ifndef OSCIT_INCLUDE_OSCIT_INFO_META_METHOD_H_
#define OSCIT_INCLUDE_OSCIT_INFO_META_METHOD_H_
#include "oscit/root.h"

namespace oscit {

class InfoMetaMethod : public Object
{
public:  
  /** Class signature. */
  TYPED("Object.InfoMetaMethod")
  
  InfoMetaMethod(const char *name) : Object(name, StringIO("path", "Return information on the given path.")) {}

  virtual const Value trigger (const Value &path) {
    if (!path.is_string()) return gNilValue;
    
    Value error;
    Object * target = root_->find_or_build_object_at(path.c_str(), &error);
    
    return target ? target->info() : error;
  }
};

} // oscit

#endif // OSCIT_INCLUDE_OSCIT_INFO_META_METHOD_H_

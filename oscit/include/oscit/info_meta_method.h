#ifndef _OSCIT_INFO_META_METHOD_H_
#define _OSCIT_INFO_META_METHOD_H_
#include "oscit/root.h"

namespace oscit {

class InfoMetaMethod : public Object
{
public:  
  /** Class signature. */
  TYPED("Object.InfoMetaMethod")
  
  InfoMetaMethod(const char *name) : Object(name, StringIO("url", "Return information on the given url.")) {}

  virtual const Value trigger (const Value &url) {
    Value error;
    Object * target = root_->find_or_build_object_at(url.c_str(), &error);
    
    return target ? target->info() : error;
  }
};

#endif // _OSCIT_INFO_META_METHOD_H_

} // oscit
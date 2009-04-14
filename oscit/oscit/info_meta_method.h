#ifndef _OSCIT_INFO_META_METHOD_H_
#define _OSCIT_INFO_META_METHOD_H_
#include "oscit/root.h"

namespace oscit {

class InfoMetaMethod : public BaseObject
{
public:
  InfoMetaMethod(const char * name) : BaseObject(name, H("s")) {}

  virtual const Value trigger (const Value &url) {
    Value error;
    BaseObject * target = root_->find_or_build_object_at(url.c_str(), &error);
    
    return target ? Value(target->info()) : error;
  }
};

#endif // _OSCIT_INFO_META_METHOD_H_

} // oscit
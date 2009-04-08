#ifndef _OSCIT_INFO_META_METHOD_H_
#define _OSCIT_INFO_META_METHOD_H_
#include "oscit/root.h"

namespace oscit {

class InfoMetaMethod : public BaseObject
{
public:
  InfoMetaMethod(const char * name) : BaseObject(name, H("s")) {}

  virtual const Value trigger (const Value &url) {
    BaseObject * target = root_->find_or_build_object_at(url.s);

    return target ? Value(target->info()) : ErrorValue(NOT_FOUND_ERROR, url.s);
  }
};

#endif // _OSCIT_INFO_META_METHOD_H_

} // oscit
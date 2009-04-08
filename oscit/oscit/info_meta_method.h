#ifndef _INFO_META_METHOD_H_
#define _INFO_META_METHOD_H_
#include "oscit/root.h"

namespace oscit {

class InfoMetaMethod : public Object
{
public:
  InfoMetaMethod(const char * name) : Object(name, H("s")) {}

  virtual const Value trigger (const Value &url) {
    Object * target = root_->find_or_build_object_at(url.s);

    return target ? Value(target->info()) : ErrorValue(NOT_FOUND_ERROR, url.s);
  }
};

#endif // _INFO_META_METHOD_H_

} // oscit
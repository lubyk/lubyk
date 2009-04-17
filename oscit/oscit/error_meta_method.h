#ifndef _OSCIT_ERROR_META_METHOD_H_
#define _OSCIT_ERROR_META_METHOD_H_
#include "oscit/root.h"

namespace oscit {

class ErrorMetaMethod : public Object
{
public:  
  /** Class signature. */
  CLASS_PATH("Object.ErrorMetaMethod")
  
  ErrorMetaMethod(const char * name) : Object(name, ANY_TYPE_TAG_ID) {}

  virtual const Value trigger (const Value &val) {
    std::cerr << val << std::endl;
    return gNilValue;
  }
};

#endif // _OSCIT_ERROR_META_METHOD_H_

} // oscit
#ifndef OSCIT_INCLUDE_OSCIT_ERROR_META_METHOD_H_
#define OSCIT_INCLUDE_OSCIT_ERROR_META_METHOD_H_
#include "oscit/root.h"

namespace oscit {

class ErrorMetaMethod : public Object
{
public:  
  /** Class signature. */
  TYPED("Object.ErrorMetaMethod")
  
  ErrorMetaMethod(const char * name) : Object(name, AnyIO("Errors returned should end here.")) {}

  virtual const Value trigger (const Value &val) {
    std::cerr << val << std::endl;
    return gNilValue;
  }
};

} // oscit

#endif // OSCIT_INCLUDE_OSCIT_ERROR_META_METHOD_H_

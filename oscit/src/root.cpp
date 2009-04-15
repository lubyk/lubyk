#include "oscit/root.h"
// meta methods
#include "oscit/info_meta_method.h"
#include "oscit/type_meta_method.h"
#include "oscit/error_meta_method.h"

namespace oscit {

void Root::init() {
  root_ = NULL;
  register_object(this);
  
  // build meta methods
  adopt(new InfoMetaMethod(".info"));
  adopt(new TypeMetaMethod(".type"));
  adopt(new ErrorMetaMethod(".error"));
}

} // namespace oscit
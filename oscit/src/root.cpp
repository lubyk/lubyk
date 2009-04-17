#include "oscit/root.h"
// meta methods
#include "oscit/info_meta_method.h"
#include "oscit/type_meta_method.h"
#include "oscit/error_meta_method.h"
#include "oscit/list_meta_method.h"
#include "oscit/tree_meta_method.h"

namespace oscit {

void Root::init() {
  root_ = NULL;
  register_object(this);
  // objects_.set("/", this); // the root object should not be accessed with "/" but with "".
  
  // build meta methods
  adopt(new ErrorMetaMethod(".error"));
  adopt(new InfoMetaMethod(".info"));
  adopt(new ListMetaMethod(".list"));
  adopt(new TypeMetaMethod(".type"));
  adopt(new TreeMetaMethod(".tree"));
}

} // namespace oscit
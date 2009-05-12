#include "oscit/root.h"
// meta methods
#include "oscit/info_meta_method.h"
#include "oscit/type_meta_method.h"
#include "oscit/error_meta_method.h"
#include "oscit/list_meta_method.h"
#include "oscit/tree_meta_method.h"

namespace oscit {

void Root::init() {
  url_ = "";
  set_root(this);
  
  // build meta methods
  adopt(new ErrorMetaMethod(".error"));
  adopt(new InfoMetaMethod(".info"));
  adopt(new ListMetaMethod(".list"));
  adopt(new TypeMetaMethod(".type"));
  adopt(new TreeMetaMethod(".tree"));
}

} // oscit

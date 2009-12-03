#include "oscit/root.h"

// meta methods
#include "oscit/info_meta_method.h"
#include "oscit/type_meta_method.h"
#include "oscit/error_meta_method.h"
#include "oscit/list_meta_method.h"
#include "oscit/list_with_type_meta_method.h"
#include "oscit/tree_meta_method.h"

namespace oscit {

void Root::init(bool should_build_meta) {
  url_ = "";
  set_root(this);

  if (should_build_meta) {
    // build meta methods
    adopt(new ErrorMetaMethod(Url(ERROR_PATH).name()));
    adopt(new InfoMetaMethod(Url(INFO_PATH).name()));
    adopt(new ListMetaMethod(Url(LIST_PATH).name()));
    adopt(new ListWithTypeMetaMethod(Url(LIST_WITH_TYPE_PATH).name()));
    adopt(new TypeMetaMethod(Url(TYPE_PATH).name()));
    adopt(new TreeMetaMethod(Url(TREE_PATH).name()));
  }
}

} // oscit

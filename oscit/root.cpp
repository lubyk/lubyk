#include "oscit/root.h"
// meta methods
#include "oscit/meta_methods/info_meta_method.h"
#include "oscit/meta_methods/type_meta_method.h"

namespace oscit {
Root::~Root() {
  std::list<OscSend*>::iterator it  = observers_.begin();
  std::list<OscSend*>::iterator end = observers_.end();

  while (it != end) delete *it++;
  
  if (osc_in_) delete osc_in_;
  clear();
  root_ = NULL; // avoid call to unregister_object in ~Object
}

// build meta methods
void Root::build_meta_methods() {
  adopt(new InfoMetaMethod(".info"));
  adopt(new TypeMetaMethod(".type"));
}
} // namespace oscit
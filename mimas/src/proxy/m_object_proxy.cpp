#include "mimas.h"
#include "m_object_proxy.h"

#include <list>

void MObjectProxy::connect(MRangeWidget *widget) {
  if (is_connected()) {
    widget->set_range(type()[1].r, type()[2].r);
  } else {
    widget->set_enabled(false);
  }
  range_widgets_.push_back(widget);
  widget->set_proxy(this);
}

void MObjectProxy::type_changed() {
  if (type().type_id() == H("fffss")) {
    MessageManagerLock mml;
    // RangeIO
    std::list<MRangeWidget*>::iterator it, end = range_widgets_.end();
    for (it = range_widgets_.begin(); it != end; ++it) {
      (*it)->set_enabled(true);
      (*it)->set_range(type()[1].r, type()[2].r);
    }
  }
}
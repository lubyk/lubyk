#include "mimas.h"
#include "m_range_widget.h"

#include "m_object_proxy.h"

void MRangeWidget::set_proxy(MObjectProxy *proxy) {
  set_and_hold(&proxy_, proxy);
}
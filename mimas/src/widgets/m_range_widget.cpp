#include "mimas.h"
#include "m_range_widget.h"

#include "m_object_proxy.h"


MRangeWidget::MRangeWidget()
    : last_drag_(0),
      min_(0.0),
      max_(1.0),
      value_(0.0),
      remote_value_(0.0),
      proxy_(NULL) {

}

void MRangeWidget::set_proxy(MObjectProxy *proxy) {
  set_and_hold(&proxy_, proxy);
}

void MRangeWidget::set_scaled_value(Real position, Real range) {
  if (!range) return;
  Real r = position * (max_ - min_) / range;
  if (r < min_) r = min_;
  if (r > max_) r = max_;
  value_ = r;
  proxy_->set_value(Value(value_));
}

void MRangeWidget::stop_drag() {
  last_drag_ = proxy_->time_ref().elapsed();
}

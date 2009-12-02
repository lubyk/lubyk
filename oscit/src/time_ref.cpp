#include "oscit/time_ref.h"
#include <sys/timeb.h> // ftime


struct TimeRef::TimeRefData : public timeb {};

TimeRef::TimeRef() {
  reference_ = new TimeRefData;
  ftime(reference_);
}

TimeRef::~TimeRef() {
  delete reference_;
}

/** Get current real time in [ms] since the time ref object was created.
 */
time_t TimeRef::elapsed() {
  TimeRefData t;
  ftime(&t);
  return ((t.time - reference_->time) * 1000) + t.millitm - reference_->millitm;
}

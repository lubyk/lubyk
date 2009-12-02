#ifndef OSCIT_INCLUDE_OSCIT_TIME_REF_H_
#define OSCIT_INCLUDE_OSCIT_TIME_REF_H_

#include <sys/types.h>  // time_t

#include "oscit/non_copyable.h"

class TimeRef : private NonCopyable {
public:
  TimeRef();
  ~TimeRef();

  /** Get current real time in [ms] since the time ref object was created.
   */
  time_t elapsed();
private:
  struct TimeRefData;
  TimeRefData *reference_;
};

#endif // OSCIT_INCLUDE_OSCIT_TIME_REF_H_
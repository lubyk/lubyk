#ifndef OSCIT_INCLUDE_OSCIT_SCALE_REAL_H_
#define OSCIT_INCLUDE_OSCIT_SCALE_REAL_H_
#include <string>

#include "oscit/conf.h"

namespace oscit {

class ScaleReal {
 public:
  ScaleReal(Real source_min, Real source_max, Real target_min, Real target_max) :
                       source_min_(source_min), target_min_(target_min),
                       target_max_(target_max) {
    assert(source_min != source_max);
    scale_ = (target_max - target_min) / (source_max - source_min);
  }
  
  Real scale(Real source_value) const {
    Real res = target_min_ + ( (source_value - source_min_) * scale_);
    return res < target_min_ ? target_min_ : (res > target_max_ ? target_max_ : res);
  }
 
 private:
  Real source_min_;
  Real target_min_;
  Real target_max_;
  Real scale_;
};

}  // oscit

#endif // OSCIT_INCLUDE_OSCIT_SCALE_REAL_H_
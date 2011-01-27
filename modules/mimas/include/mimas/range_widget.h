/*
  ==============================================================================

   This file is part of the LUBYK project (http://lubyk.org)
   Copyright (c) 2007-2011 by Gaspard Bucher (http://teti.ch).

  ------------------------------------------------------------------------------

   Mimas is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Mimas is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Mimas.  If not, see <http://www.gnu.org/licenses/>.

  ==============================================================================
*/

#ifndef LUBYK_INCLUDE_MIMAS_RANGE_WIDGET_H_
#define LUBYK_INCLUDE_MIMAS_RANGE_WIDGET_H_

#include "mimas/mimas.h"

namespace mimas {

class RangeWidget
{
public:
  RangeWidget()
   : min_(0.0),
     max_(1.0),
     value_(0.0),
     remote_value_(0.0),
     is_dragged_(false) {}

  virtual ~RangeWidget() {}

  double value() const {
    return value_;
  }

  void setRange(double min, double max) {
    min_ = min;
    max_ = max;
  }

  /** Internal. Used by the GUI to get the local position (knob). */
  int position(double range) {
    if (max_ - min_ == 0) return 0;
    return (int)((value_ * range / (max_ - min_)) + 0.5f);
  }

  /** Internal. Used by the GUI to get remote position.
   */
  int remotePosition(double range) {
    if (max_ - min_ == 0) return 0;
    return (int)((remote_value_ * range / (max_ - min_)) + 0.5f);
  }

  /** Internal. Called by the GUI on drag to change the
   * remote value.
   * @return true if the value changed and we should emit.
   */
  bool setPosition(double position, double range) {
    if (!range) return false;
    double r = position * (max_ - min_) / range;
    if (r < min_) r = min_;
    if (r > max_) r = max_;
    if (value_ != r) {
      value_ = r;
      return true;
    }
    return false;
  }

  bool isDragged() const {
    return is_dragged_;
  }

  void setDragged(bool dragged) {
    is_dragged_ = dragged;
  }

  /** Called by owner to set new value.
   *
   * @return true if the value changed and we should repaint.
   */
  bool setValue(double remote_value) {
    if (remote_value_ != remote_value) {
      remote_value_ = remote_value;

      if (!is_dragged_) {
        // no dragging
        //if (last_drag_ < 0 || last_drag_ + (3 * proxy_->latency()) < proxy_->elapsed()) {
          // we stopped dragging long ago, move real.
          value_ = remote_value;
        //}
      }

      return true;
    }
    return false;
  }

private:
  /** Smallest possible value.
   */
  double min_;

  /** Largest possible value.
   */
  double max_;

  /** Current value set by user.
   */
  double value_;

  /** Remote value.
   */
  double remote_value_;

  /** Set to true if the element is being dragged.
   */
  bool is_dragged_;
};

} // mimas

#endif // LUBYK_INCLUDE_MIMAS_RANGE_WIDGET_H_
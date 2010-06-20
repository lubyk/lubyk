/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
   Copyright (c) 2007-2010 by Gaspard Bucher - Buma (http://teti.ch).

  ------------------------------------------------------------------------------

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

  ==============================================================================
*/

#ifndef RUBYK_INCLUDE_RUBYK_NODE_VIEW_H_
#define RUBYK_INCLUDE_RUBYK_NODE_VIEW_H_

namespace rk {

#define POS_WIDTH  "width"
#define POS_HEIGHT "height"
#define POS_HUE "hue"
#define POS_X "x"
#define POS_Y "y"

struct NodeView {
  NodeView(Real x = 0, Real y = 0, Real width = 60, Real height = 20, Real hue = 203)
    : x_(x), y_(y), width_(width), height_(height), hue_(hue) {}

  void from_hash(const Value &hash, Value *result) {
    if (!hash.is_hash()) return;

    Value tmp;

    if (hash.get(POS_X, &tmp) && tmp.is_real()) {
      x_ = tmp.r;
      result->set(POS_X, x_);
    }

    if (hash.get(POS_Y, &tmp) && tmp.is_real()) {
      y_ = tmp.r;
      result->set(POS_Y, y_);
    }

    if (hash.get(POS_WIDTH, &tmp) && tmp.is_real()) {
      width_ = tmp.r;
      result->set(POS_WIDTH, width_);
    }

    if (hash.get(POS_HEIGHT, &tmp) && tmp.is_real()) {
      height_ = tmp.r;
      result->set(POS_HEIGHT, height_);
    }

    if (hash.get(POS_HUE, &tmp) && tmp.is_real()) {
      hue_ = tmp.r;
      result->set(POS_HUE, hue_);
    }
  }

  void insert_in_hash(Value *result) const {
    result->set(POS_X, x_);
    result->set(POS_Y, y_);
    result->set(POS_WIDTH, width_);
    result->set(POS_HEIGHT, height_);
    result->set(POS_HUE, hue_);
  }

  const Value to_hash() const {
    Value result;
    insert_in_hash(&result);

    return result;
  }

  /** Object horizontal position.
   */
  Real x_;

  /** Object vertical position.
   */
  Real y_;

  /** Object width.
   */
  Real width_;

  /** Object height.
   */
  Real height_;

  /** Object color (hue).
   */
  Real hue_;
};

inline std::ostream &operator<<(std::ostream &out_stream, const NodeView &view) {
  out_stream << view.to_hash().to_json();
  return out_stream;
}

} // rk

#endif // RUBYK_INCLUDE_RUBYK_NODE_VIEW_H_
/*
  ==============================================================================

   This file is part of the LUBYK project (http://lubyk.org)
   Copyright (c) 2007-2011 by Gaspard Bucher (http://teti.ch).

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
#ifndef LUBYK_INCLUDE_MIMAS_COLOR_H_
#define LUBYK_INCLUDE_MIMAS_COLOR_H_

#include "mimas/mimas.h"
#include <QtGui/QColor>

#include <iostream>

namespace mimas {

/** Color used by Painter.
 *
 */
class Color : public QColor
{
public:
  Color() {}

  /** Create a color from a given hue, saturation, value and alpha.
   * All values are in the range 0..1
   */
  Color(float h, float s=1.0, float v=1.0, float a=1.0) {
    setHsvF(h, s, v, a);
  }

  ~Color() {
    MIMAS_DEBUG_GC
  }

  float hue() const {
    return hueF();
  }

  float saturation() const {
    return saturationF();
  }

  float value() const {
    return valueF();
  }

  float alpha() const {
    return alphaF();
  }

  void setRgba(float r, float g, float b, float a=1.0) {
    setRgbF(r, g, b, a);
  }

  void setHsva(float h, float s=1.0, float v=1.0, float a=1.0) {
    setHsvF(h, s, v, a);
  }

  void setHue(float h) {
    setHsvF(h, saturationF(), valueF(), alphaF());
  }

  void setSaturation(float s) {
    setHsvF(hueF(), s, valueF(), alphaF());
  }

  void setValue(float v) {
    setHsvF(hueF(), saturationF(), v, alphaF());
  }

  void setAlpha(float a) {
    setHsvF(hueF(), saturationF(), valueF(), a);
  }

  Color *colorWithHue(float h) {
    return new Color(h, saturationF(), valueF(), alphaF());
  }

  Color *colorWithSaturation(float s) {
    return new Color(hueF(), s, valueF(), alphaF());
  }

  Color *colorWithValue(float v) {
    return new Color(hueF(), saturationF(), v, alphaF());
  }

  Color *colorWithAlpha(float a) {
    return new Color(hueF(), saturationF(), valueF(), a);
  }
};

} // mimas
#endif // LUBYK_INCLUDE_MIMAS_COLOR_H_

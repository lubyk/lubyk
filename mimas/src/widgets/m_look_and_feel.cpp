/*
  ==============================================================================

   This file is part of the MIMAS project (http://rubyk.org/mimas)
   Copyright (c) 2007-2009 by Gaspard Bucher - Buma (http://teti.ch).

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

#include "m_look_and_feel.h"
//==============================================================================
static const Colour createBaseColour (const Colour& buttonColour,
                                      const bool hasKeyboardFocus,
                                      const bool isMouseOverButton,
                                      const bool isButtonDown) throw()
{
    const float sat = hasKeyboardFocus ? 1.3f : 0.9f;
    const Colour baseColour (buttonColour.withMultipliedSaturation (sat));

    if (isButtonDown)
        return baseColour.contrasting (0.2f);
    else if (isMouseOverButton)
        return baseColour.contrasting (0.1f);

    return baseColour;
}

//==============================================================================
void MLookAndFeel::drawLinearSlider (Graphics& g,
                                    int x, int y,
                                    int width, int height,
                                    float sliderPos,
                                    float minSliderPos,
                                    float maxSliderPos,
                                    const Slider::SliderStyle style,
                                  Slider& slider) {

  if (style == Slider::LinearBar) {
    LookAndFeel::drawLinearSlider(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
  } else {
    g.fillAll (slider.findColour (Slider::backgroundColourId));

    drawLinearSliderBackground (g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);

    double real_value = slider.getLinearSliderPos(slider.getComponentPropertyDouble(T("RemoteValue"), false));
    if (real_value - sliderPos > 3 || real_value - sliderPos < -3) {
      drawGhostSliderThumb  (g, x, y, width, height, real_value,  minSliderPos, maxSliderPos, style, slider);
    }
    drawLinearSliderThumb (g, x, y, width, height, sliderPos,  minSliderPos, maxSliderPos, style, slider);
  }
}

void MLookAndFeel::drawGhostSliderThumb (Graphics& g,
                                         int x, int y,
                                         int width, int height,
                                         float sliderPos,
                                         float minSliderPos,
                                         float maxSliderPos,
                                         const Slider::SliderStyle style,
                                         Slider& slider) {
  const float sliderRadius = (float) (getSliderThumbRadius (slider) - 2);

  Colour knobColour (createBaseColour (slider.findColour (Slider::thumbColourId),
                                       slider.hasKeyboardFocus (false) && slider.isEnabled(),
                                       slider.isMouseOverOrDragging() && slider.isEnabled(),
                                       slider.isMouseButtonDown() && slider.isEnabled()).withAlpha(0.5f));

  const float outlineThickness = slider.isEnabled() ? 0.8f : 0.3f;

  float kx, ky;

  if (style == Slider::LinearVertical)
  {
    kx = x + width * 0.5f;
    ky = sliderPos;
  }
  else
  {
    kx = sliderPos;
    ky = y + height * 0.5f;
  }

  drawGlassSphere (g,
    kx - sliderRadius,
    ky - sliderRadius,
    sliderRadius * 2.0f,
    knobColour, outlineThickness);
}
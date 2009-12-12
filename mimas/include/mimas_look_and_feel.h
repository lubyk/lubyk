#ifndef MIMAS_SRC_CORE_MIMAS_LOOK_AND_FEEL_H_
#define MIMAS_SRC_CORE_MIMAS_LOOK_AND_FEEL_H_
#include "mimas.h"

class MimasLookAndFeel : public LookAndFeel {
public:
  virtual void drawLinearSlider (Graphics& g,
                                      int x, int y,
                                      int width, int height,
                                      float sliderPos,
                                      float minSliderPos,
                                      float maxSliderPos,
                                      const Slider::SliderStyle style,
                                      Slider& slider);
protected:
  virtual void drawGhostSliderThumb (Graphics& g,
                                       int x, int y,
                                       int width, int height,
                                       float sliderPos,
                                       float minSliderPos,
                                       float maxSliderPos,
                                       const Slider::SliderStyle style,
                                       Slider& slider);

};
#endif // MIMAS_SRC_CORE_MIMAS_LOOK_AND_FEEL_H_
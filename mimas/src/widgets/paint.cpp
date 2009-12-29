#include "mimas.h"
#include "m_slider.h"
#include "m_pad.h"

#include "m_object_proxy.h"

#define SLIDER_BORDER_WIDTH 1


// =============================================
// ==             MSlider                     ==
// =============================================

void MSlider::paint(Graphics &g) {
  g.fillAll(Colours::black);
  g.setColour(Colours::pink);
  if (slider_type_ == VerticalSliderType) {
    // vertical slider
    int remote_pos = scaled_remote_value(getHeight());
    g.fillRect(0, getHeight() - remote_pos, getWidth(), remote_pos);
    //if (remote_pos - position > 3 || remote_pos - position < -3) {
      // TODO: should be if dragged_ || last_drag ...
      // draw ghost
    //}
  } else {
    // horizontal slider
    int remote_pos = scaled_remote_value(getWidth());
    g.fillRect(0, 0, remote_pos, getHeight());
  }
  g.setColour(Colours::darkgrey);
  g.drawRect(0, 0, getWidth(), getHeight());
}

// =============================================
// ==             MPad                        ==
// =============================================

void MPad::paint(Graphics& g) {
  float radius = 8;
  float pos_x;
  float pos_y;

  g.fillAll(Colours::grey);

  if (abs(range_x_.value_ - range_x_.remote_value_) + abs(range_y_.value_ - range_y_.remote_value_) > 4 * radius) {
    // remote_value_
    pos_x = range_x_.scaled_remote_value(getWidth()) - radius;
    pos_y = getHeight() - range_y_.scaled_remote_value(getHeight()) - radius;
    g.setColour(Colours::lightgrey);
    g.fillEllipse(pos_x, pos_y, 2*radius, 2*radius);

    g.setColour(Colours::darkgrey);
    g.drawEllipse(pos_x, pos_y, 2*radius, 2*radius, 2.0f);
  }

  // value_
  pos_x = range_x_.scaled_value(getWidth())  - radius;
  pos_y = getHeight() - range_y_.scaled_value(getHeight()) - radius;
  g.setColour(Colours::white);
  g.fillEllipse(pos_x, pos_y, 2*radius, 2*radius);

  g.setColour(Colours::black);
  g.drawEllipse(pos_x, pos_y, 2*radius, 2*radius, 2.0f);
}

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

#include "mimas.h"
#include "mimas_window_content.h"
#include "m_browser.h"
#include "m_device_list.h"
#include "m_breadcrumbs.h"
#include "m_breadcrumbs_item.h"
#include "m_path_list.h"
#include "m_ghost_component.h"
#include "m_device_view.h"
#include "m_device_label.h"
#include "m_slider.h"
#include "m_pad.h"
#include "m_theme.h"

#include "m_object_proxy.h"

#define SLIDER_BORDER_WIDTH 2
#define SLIDER_BORDER_RADIUS 6


// =============================================
// ==             Colors                      ==
// =============================================

void MTheme::set_day_theme() {
  // TODO: define a day theme...
  set_night_theme();
}

void MTheme::set_night_theme() {
  colors_[WorkspaceActionBG] = Colour(0xff262626);
  colors_[WorkspaceEditBG]   = Colour(0xff262626);
  colors_[WorkspaceFrozenBG] = Colour(0xff3f0000);
  colors_[WorkspaceBorder]   = Colour(0xff000000);

  colors_[Disconnected]      = Colour(0x10ff0000);

  colors_[GhostBG]           = Colour(0x40ffffff);
  colors_[GhostBorder]       = Colour(0x10ffffff);

  colors_[BrowserSelectedItem] = Colour(0xff303030);
  colors_[BrowserLabel]      = Colour(0xffa0a0a0);

  colors_[BreadcrumbsBG]     = Colour(0xff303030);
  colors_[BreadcrumbsLabel]  = Colour(0xffa0a0a0);

  colors_[ToolbarBG]         = Colour(0xff808080);
  colors_[DeviceBorder]      = Colour(0xffa0a0a0);
  colors_[DeviceLabel]       = Colour(0xffa0a0a0);

  font_size_ = 16.0f;
}


// =============================================
// ==             MimasWindowContent          ==
// =============================================

#define FID_SIZE 8
#define FID_SPACE 100
void MimasWindowContent::paint(Graphics& g) {
  // TODO: move this in the draw code for the workspace_, not the view port ...
  g.fillAll(bg_color());

  if (editor_mode_ == EditMode) {
    Image* bg_image = ImageCache::getFromFile(juce::File(T("/Users/gaspard/git/rubyk/mimas/doc/edit_bg.png")));
    g.setTiledImageFill(*bg_image, 0, 0, 1.0f);
    g.fillAll();
  }
  g.setColour(color(MTheme::ToolbarBG));
  g.fillRect(0, 0, getWidth(), TOOLBAR_HEIGHT);
  //g.strokePath (internalPath1, PathStrokeType (5.2000f));
}


// =============================================
// ==             MGhostComponent             ==
// =============================================

void MGhostComponent::paint(Graphics &g) {
  g.fillAll(mimas_->color(MTheme::GhostBG));
  g.setColour(mimas_->color(MTheme::GhostBorder));
  g.drawRect(
    0,
    0,
    getWidth(),
    getHeight(),
    SLIDER_BORDER_WIDTH
  );
}

// =============================================
// ==             MBrowser                    ==
// =============================================

void MBrowser::paint(Graphics &g) {
  g.fillAll(mimas_->bg_color());
  g.setColour(mimas_->color(MTheme::DeviceBorder));
  g.drawLine(
    getWidth() - LAYOUT_BROWSER_BORDER_WIDTH / 2.0f,
    0,
    getWidth() - LAYOUT_BROWSER_BORDER_WIDTH / 2.0f,
    getHeight(),
    LAYOUT_BROWSER_BORDER_WIDTH
  );

  g.setColour(mimas_->bg_color());
  g.drawLine(
    getWidth() - LAYOUT_BROWSER_BORDER_WIDTH / 2.0f,
    (getHeight() / 2) - 2 * LAYOUT_BROWSER_BORDER_WIDTH,
    getWidth() - LAYOUT_BROWSER_BORDER_WIDTH / 2.0f,
    (getHeight() / 2) + 2 * LAYOUT_BROWSER_BORDER_WIDTH,
    1.0f
  );
}


// =============================================
// ==             MDeviceList                 ==
// =============================================

void MDeviceList::paintListBoxItem(int rowNumber, Graphics& g, int width, int height, bool rowIsSelected) {
  if (rowIsSelected) g.fillAll(mimas_->color(MTheme::BrowserSelectedItem));

  g.setColour(mimas_->color(MTheme::BrowserLabel));
  g.setFont(mimas_->font_size());

  g.drawText(
    String(devices_[rowNumber]->remote_location().name().c_str()),
    5, 0, width, height - 1.0f,
    Justification::centredLeft,
    true
  );

  g.drawLine(
    0,
    height,
    width,
    height,
    1
  );
}

void MDeviceList::paint(Graphics &g) {
  setColour(ListBox::backgroundColourId, mimas_->bg_color());
  setColour(ListBox::textColourId, mimas_->color(MTheme::BrowserLabel));
}

// =============================================
// ==             MBreadcrumbs                ==
// =============================================

void MBreadcrumbs::paint(Graphics &g) {
  g.fillAll(mimas_->bg_color());
  g.setColour(mimas_->color(MTheme::DeviceBorder));
  g.drawLine(
    0,
    BREADCRUMBS_BORDER_WIDTH / 2,
    getWidth(),
    BREADCRUMBS_BORDER_WIDTH / 2,
    BREADCRUMBS_BORDER_WIDTH
  );

  g.drawLine(
    0,
    getHeight() - BREADCRUMBS_BORDER_WIDTH / 2,
    getWidth(),
    getHeight() - BREADCRUMBS_BORDER_WIDTH / 2,
    BREADCRUMBS_BORDER_WIDTH
  );
}

// =============================================
// ==             MBreadcrumbsItem            ==
// =============================================

void MBreadcrumbsItem::paint(Graphics &g) {
  int width  = getWidth();
  int height = getHeight();
  int half_height = height / 2;

  // Fill rectangle where the text lies
  g.setColour(mimas_->color(MTheme::BreadcrumbsBG));
  g.fillRect(
    0,
    0,
    width - half_height,
    height
  );

  // FIXME: cache juce::Paths
  Path arrow_fill;

  // draw front arrow >
  arrow_fill.startNewSubPath(width - half_height, -1);
  arrow_fill.lineTo(width, half_height);
  arrow_fill.lineTo(width - half_height, height + 1);
  Path arrow(arrow_fill); // copy

  // fill arrow
  arrow_fill.closeSubPath();
  g.fillPath(arrow_fill);

  // arrow stroke
  g.setColour(mimas_->color(MTheme::DeviceBorder));
  g.strokePath(arrow, 1);

  // draw label
  g.setColour(mimas_->color(MTheme::BreadcrumbsLabel));
  g.setFont(mimas_->font_size());
  g.drawText(
    String(url_.name().c_str()),
    half_height + 5, 0, width, height,
    Justification::centredLeft,
    true
  );
}

int MBreadcrumbsItem::min_width() {
  return Font(mimas_->font_size()).getStringWidth(String(url_.name().c_str())) + 5; // FIXME: margin
}

// =============================================
// ==             MPathList                   ==
// =============================================

void MPathList::paintListBoxItem(int rowNumber, Graphics& g, int width, int height, bool rowIsSelected) {
  if (rowIsSelected) g.fillAll(mimas_->color(MTheme::BrowserSelectedItem));

  g.setColour(mimas_->color(MTheme::BrowserLabel));
  g.setFont(mimas_->font_size());
  g.drawText(
    String(container_->child_at_index(rowNumber)->name().c_str()),
    5, 0, width, height,
    Justification::centredLeft,
    true
  );


  g.drawLine(
    0,
    height,
    width,
    height,
    1
  );
}

void MPathList::paint(Graphics &g) {
  setColour(ListBox::backgroundColourId, mimas_->bg_color());
  setColour(ListBox::textColourId, mimas_->color(MTheme::BrowserLabel));
}

// =============================================
// ==             MDeviceView                 ==
// =============================================

void MDeviceView::paint(Graphics &g) {
  g.setColour(mimas_->color(MTheme::DeviceBorder).withMultipliedAlpha(hover_ ? 1.0f : 0.5f));

  g.drawRoundedRectangle(
    (DEVICE_BORDER_WIDTH / 2),
    (DEVICE_BORDER_WIDTH / 2) + label_->min_height() / 2,
    getWidth() - DEVICE_BORDER_WIDTH,
    getHeight() - (label_->min_height() / 2) - DEVICE_BORDER_WIDTH,
    DEVICE_ROUNDED_RADIUS,
    DEVICE_BORDER_WIDTH);
}

// =============================================
// ==             MDeviceLabel                ==
// =============================================

void MDeviceLabel::paint(Graphics &g) {
  label_->setColour(Label::textColourId, mimas_->color(MTheme::DeviceLabel));
  label_->setColour(Label::backgroundColourId, mimas_->bg_color());
  Component::paint(g);
}

// =============================================
// ==             MSlider                     ==
// =============================================

/**
 * TODO: use a table lookup for the round_delta ?
 */

void MSlider::paint(Graphics &g) {
  if (isEnabled()) {
    g.fillAll(mimas_->bg_color());
  } else {
    g.fillAll(mimas_->color(MTheme::Disconnected));
  }

  g.setColour(fill_color_);
  if (slider_type_ == VerticalSliderType) {
    // vertical slider
    int remote_pos = scaled_remote_value(getHeight()-2);  // TODO: -2 ?
    int h = getHeight() - remote_pos - SLIDER_BORDER_WIDTH / 2;
    // filled slider value
    g.fillRect(
      0,
      h,
      getWidth(),
      remote_pos
    );
    // line on top of value
    g.setColour(border_color_);
    //g.drawLine(
    //  SLIDER_BORDER_WIDTH / 2,
    //  h - SLIDER_BORDER_WIDTH / 2,
    //  getWidth() - SLIDER_BORDER_WIDTH,
    //  h - SLIDER_BORDER_WIDTH / 2,
    //  SLIDER_BORDER_WIDTH
    //);
  } else {
    // horizontal slider
    int remote_pos = scaled_remote_value(getWidth()-2);
    g.fillRect(0, 0, remote_pos, getHeight());
  }
  g.setColour(border_color_);
  g.drawRect(
    0,
    0,
    getWidth(),
    getHeight(),
    SLIDER_BORDER_WIDTH
  );
}

// =============================================
// ==             MPad                        ==
// =============================================

void MPad::paint(Graphics& g) {
  if (isEnabled()) {
    g.fillAll(mimas_->bg_color());
  } else {
    g.fillAll(mimas_->color(MTheme::Disconnected));
  }

  // if (abs(range_x_.value_ - range_x_.remote_value_) + abs(range_y_.value_ - range_y_.remote_value_) > 4 * radius) {
  //   // draw ghost
  // }

  // vertical slider
  int remote_x = range_x_.scaled_remote_value(getWidth()-2); // TODO: -2 ?
  int remote_y = getHeight() - range_y_.scaled_remote_value(getHeight()-2) - SLIDER_BORDER_WIDTH / 2;

  // filled pad value
  g.setColour(fill_color_);
  g.fillRect(
    0,
    remote_y,
    remote_x,
    getHeight()
  );

  // pad border
  g.setColour(border_color_);
  g.drawRect(
    0,
    0,
    getWidth(),
    getHeight(),
    SLIDER_BORDER_WIDTH
  );
}

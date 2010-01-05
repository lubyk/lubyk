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

#include "mimas_window_content.h"
#include "m_theme.h"

#define LAYOUT_SCROLLBAR_THICKNESS 8

MimasWindowContent::MimasWindowContent()
    : editor_mode_(ActionMode),
      is_day_mode_(false),
      selected_object_(NULL),
      work_tree_("MimasWindowContent"),
      toolbar_(NULL),
      device_browser_(NULL),
      workspace_(NULL),
      workspace_port_(NULL) {

  themes_ = new MTheme[2];
  themes_[0].set_night_theme();
  themes_[1].set_day_theme();

  // no service-type: we do not want to publish a service
  OscCommand *cmd = work_tree_.adopt_command(new OscCommand("oscit"));


  border_ = new ResizableBorderComponent(this, 0);
  addAndMakeVisible(border_);

  workspace_ = new Component;
  workspace_port_ = new Viewport;
  workspace_->setSize(2000,2000);
  workspace_port_->setViewedComponent(workspace_);
  workspace_port_->setScrollBarThickness(LAYOUT_SCROLLBAR_THICKNESS);
  addAndMakeVisible(workspace_port_);

  device_browser_ = new MBrowser(this, "_oscit._udp");
  device_browser_->adopt_proxy_factory(new MProxyFactory(this, workspace_));
  // FIXME: it should be cmd->adopt_browser(device_browser_);
  device_browser_->set_command(cmd);
  device_browser_->setBounds(0, 0, LAYOUT_BROWSER_WIDTH + LAYOUT_BROWSER_BORDER_WIDTH, getHeight());
  addAndMakeVisible(device_browser_);

  toolbar_ = new Component;
  addAndMakeVisible(toolbar_);

  edit_mode_button_ = new TextButton(T("lock button"), T("lock"));
  edit_mode_button_->addButtonListener(this);
  edit_mode_button_->setBounds(2, 2, 100, 20);
  toolbar_->addAndMakeVisible(edit_mode_button_);

  setSize(600, 600);
  resized();
}

MimasWindowContent::~MimasWindowContent() {
  deleteAndZero(device_browser_);
  deleteAndZero(workspace_port_);
  delete[] themes_;
}

void MimasWindowContent::resized() {
  resize_except(NULL);
}

void MimasWindowContent::resize_except(Component *skip_component) {
  float browser_width = device_browser_->getWidth();
  if (skip_component != toolbar_ && toolbar_)
    toolbar_->setBounds(0, 0, getWidth(), TOOLBAR_HEIGHT);

  if (skip_component != device_browser_ && device_browser_)
    device_browser_->setBounds(0, TOOLBAR_HEIGHT, browser_width, getHeight() - TOOLBAR_HEIGHT);

  if (skip_component != border_ && border_)
    border_->setBounds(0, TOOLBAR_HEIGHT, getWidth(), getHeight() - TOOLBAR_HEIGHT);

  if (skip_component != workspace_port_ && workspace_port_)
    workspace_port_->setBounds(browser_width, TOOLBAR_HEIGHT, getWidth() - browser_width, getHeight() - TOOLBAR_HEIGHT);

}

void MimasWindowContent::buttonClicked(Button *button) {
  if (button == edit_mode_button_) {
    editor_mode_ = (EditorMode)((editor_mode_ + 1) % LastMode);
    repaint();
  }
}

void MimasWindowContent::select_object(Object *object) {
  // FIXME: do not store select_object but load it in some kind of editor/viewer...
  selected_object_ = object;
}
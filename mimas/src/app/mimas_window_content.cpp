/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  14 Apr 2008 11:28:13 am

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.11

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#include "MimasWindowContent.h"

#define TOOLBAR_HEIGHT 30
//==============================================================================
MimasWindowContent::MimasWindowContent()
    : work_tree_("MimasWindowContent"),
      device_browser_(NULL),
      workspace_(NULL)
{
  LookAndFeel::setDefaultLookAndFeel(&mimas_look_and_feel_);

  // no service-type: we do not want to publish a service
  OscCommand *cmd = work_tree_.adopt_command(new OscCommand("oscit", "", 7019)); // FIXME: no port => take any...

  border_ = new ResizableBorderComponent(this, 0);
  addAndMakeVisible(border_);

  workspace_ = new Component;
  workspace_port_ = new Viewport;
  workspace_->setSize(2000,2000);
  workspace_port_->setViewedComponent(workspace_);
  addAndMakeVisible(workspace_port_);

  device_browser_ = new DeviceBrowser("_oscit._udp");
  device_browser_->set_command(cmd);
  device_browser_->adopt_proxy_factory(new MProxyFactory(workspace_));
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
}

//==============================================================================
void MimasWindowContent::paint(Graphics& g) {
  g.fillAll(MAIN_BG_COLOR);
  g.setColour(Colours::lightgrey);
  g.fillRect(0, 0, getWidth(), TOOLBAR_HEIGHT);
  //g.strokePath (internalPath1, PathStrokeType (5.2000f));
}

void MimasWindowContent::resized() {
  float browser_width = device_browser_->getWidth();
  toolbar_->setBounds(0, 0, getWidth(), TOOLBAR_HEIGHT);

  device_browser_->setBounds(0, TOOLBAR_HEIGHT, browser_width, getHeight() - TOOLBAR_HEIGHT);
  border_->setBounds(0, TOOLBAR_HEIGHT, getWidth(), getHeight() - TOOLBAR_HEIGHT);
  workspace_port_->setBounds(browser_width, TOOLBAR_HEIGHT, getWidth() - browser_width, getHeight() - TOOLBAR_HEIGHT);
}


void MimasWindowContent::buttonClicked(Button *button) {
  if (button == edit_mode_button_) {
    // xyz
  }
}
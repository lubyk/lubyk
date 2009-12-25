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

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
    : work_tree_("Mimas"),
      device_browser_(NULL),
      main_view_(NULL)
{
  LookAndFeel::setDefaultLookAndFeel(&mimas_look_and_feel_);

  // no service-type: we do not want to publish a service
  OscCommand *cmd = work_tree_.adopt_command(new OscCommand("oscit", "", 7019)); // FIXME: no port => take any...

  border_ = new ResizableBorderComponent(this, 0);
  addAndMakeVisible(border_);

  main_view_ = new Component;
  main_port_ = new Viewport;
  main_view_->setSize(2000,2000);
  main_port_->setViewedComponent(main_view_);
  addAndMakeVisible(main_port_);

  device_browser_ = new DeviceBrowser("_oscit._udp");
  device_browser_->set_command(cmd);
  device_browser_->adopt_proxy_factory(new MProxyFactory(main_view_));
  device_browser_->setBounds(0, 0, LAYOUT_BROWSER_WIDTH + LAYOUT_BROWSER_BORDER_WIDTH, getHeight());
  addAndMakeVisible(device_browser_);

  setSize(600, 600);
  resized();
}

MainComponent::~MainComponent() {
  deleteAndZero(device_browser_);
  deleteAndZero(main_port_);
}

//==============================================================================
void MainComponent::paint(Graphics& g) {
  g.fillAll(MAIN_BG_COLOR);
  //g.strokePath (internalPath1, PathStrokeType (5.2000f));
}

void MainComponent::resized() {
  float browser_width = device_browser_->getWidth();
  device_browser_->setBounds(0, 0, browser_width, getHeight());
  border_->setBounds(0, 0, getWidth(), getHeight());
  main_port_->setBounds(browser_width, 0, getWidth() - browser_width, getHeight());
}

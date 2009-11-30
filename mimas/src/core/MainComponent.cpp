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
      quitButton (0)
{
  addAndMakeVisible (quitButton = new TextButton (String::empty));
  quitButton->setButtonText (T("Quit"));
  quitButton->addButtonListener (this);

  // no service-type: we do not want to publish a service
  OscCommand *cmd = work_tree_.adopt_command(new OscCommand("oscit", "", 7019));
  device_browser_ = new DeviceBrowser("_oscit._udp");
  device_browser_->set_command(cmd);
  device_browser_->adopt_proxy_factory(new BrowserProxyFactory);
  addAndMakeVisible(device_browser_);

  setSize(600, 600);
  resized();
}

MainComponent::~MainComponent() {
  deleteAndZero (device_browser_);
  deleteAndZero (quitButton);
}

//==============================================================================
void MainComponent::paint(Graphics& g) {
    g.fillAll (Colour (0xffc1d0ff));

    g.setColour (Colours::white);
    g.fillPath (internalPath1);
    g.setColour (Colour (0xff6f6f6f));
    g.strokePath (internalPath1, PathStrokeType (5.2000f));
}

void MainComponent::resized() {
    device_browser_->setBounds (10, 10, getWidth() - 10, getHeight() - 100);
    quitButton->setBounds (getWidth() - 176, getHeight() - 60, 120, 32);
}

void MainComponent::buttonClicked(Button* buttonThatWasClicked) {
    if (buttonThatWasClicked == quitButton)
    {
        JUCEApplication::quit();
    }
}

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

#ifndef __JUCER_HEADER_MAINCOMPONENT_MAINCOMPONENT_D0F6CD31__
#define __JUCER_HEADER_MAINCOMPONENT_MAINCOMPONENT_D0F6CD31__

#include "mimas.h"
#include "m_look_and_feel.h"
#include "oscit/osc_command.h"
#include "device_browser.h"
#include "root_proxy_tree_item.h"
#include "m_root_proxy.h"
#include "m_proxy_factory.h"

class MainComponent  : public Component {
public:
    //==============================================================================
    MainComponent ();
    ~MainComponent();

    void paint(Graphics& g);
    void resized();

    juce_UseDebuggingNewOperator

private:
    MLookAndFeel mimas_look_and_feel_;
    Root work_tree_;
    DeviceBrowser *device_browser_;
    ResizableBorderComponent *border_;

    Component *main_view_;
    Viewport *main_port_;

    // (prevent copy constructor and operator= being generated..)
    MainComponent (const MainComponent&);
    const MainComponent& operator= (const MainComponent&);
};


#endif   // __JUCER_HEADER_MAINCOMPONENT_MAINCOMPONENT_D0F6CD31__

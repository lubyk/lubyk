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

class MainComponent;

class DummyBrowser : public ZeroConfBrowser {
public:
  DummyBrowser(Label *label, const char *service_type) : ZeroConfBrowser(service_type), label_(label) {}
	
  virtual void add_device(const char *name, const char *host, unsigned int port, bool more_coming);
  virtual void remove_device(const char *name, const char *host, unsigned int port, bool more_coming);
private:
  Label *label_;
  bool was_more_coming_;
};
	

class MainComponent  : public Component,
                       public ButtonListener {
public:
    //==============================================================================
    MainComponent ();
    ~MainComponent();

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);


    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    DummyBrowser *dummy_browser_;

    //==============================================================================
    Label* helloWorldLabel;
    TextButton* quitButton;
    Path internalPath1;

    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    MainComponent (const MainComponent&);
    const MainComponent& operator= (const MainComponent&);
};


#endif   // __JUCER_HEADER_MAINCOMPONENT_MAINCOMPONENT_D0F6CD31__

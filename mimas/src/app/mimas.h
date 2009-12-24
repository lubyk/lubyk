// ======================= JUCE STUFF =======================

// This file lets us set up any special config that we need for this app..
#include "juce_AppConfig.h"

// And this includes all the juce headers..
#include "../../lib/juce/juce_amalgamated.h"

// ======================= /JUCE STUFF =======================

#include "oscit/oscit.h"
using namespace oscit;

// ======================= GUI constants

// dark background
// #define MAIN_BG_COLOR Colour(0xff222222)
// #define MAIN_LABEL_COLOR Colour(0xffaaaaaa)

// white background
#define MAIN_BG_COLOR Colours::white
#define MAIN_LABEL_COLOR Colours::darkgrey
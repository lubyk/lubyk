#include "mimas/constants.h"

#include "lua_cpp_helper.h"

using namespace mimas;



// Register namespace

static const struct luaL_Reg mimas_functions[] = {

  {NULL, NULL},
};


static const struct lua_constants_Reg mimas_namespace_constants[] = {
  {"NoBrush"                       , mimas::NoBrush},
  {"SolidPattern"                  , mimas::SolidPattern},
  {"LinearGradientPattern"         , mimas::LinearGradientPattern},
  {"ConicalGradientPattern"        , mimas::ConicalGradientPattern},
  {"RadialGradientPattern"         , mimas::RadialGradientPattern},
  {"TexturePattern"                , mimas::TexturePattern},
  {"NoPen"                         , mimas::NoPen},
  {"SolidLine"                     , mimas::SolidLine},
  {"DashLine"                      , mimas::DashLine},
  {"DotLine"                       , mimas::DotLine},
  {"DashDotLine"                   , mimas::DashDotLine},
  {"DashDotDotLine"                , mimas::DashDotDotLine},
  {"CustomDashLine"                , mimas::CustomDashLine},
  {"FlatCap"                       , mimas::FlatCap},
  {"SquareCap"                     , mimas::SquareCap},
  {"RoundCap"                      , mimas::RoundCap},
  {"MiterJoin"                     , mimas::MiterJoin},
  {"BevelJoin"                     , mimas::BevelJoin},
  {"RoundJoin"                     , mimas::RoundJoin},
  {"SvgMiterJoin"                  , mimas::SvgMiterJoin},
  {"AlignLeft"                     , mimas::AlignLeft},
  {"AlignRight"                    , mimas::AlignRight},
  {"AlignHCenter"                  , mimas::AlignHCenter},
  {"AlignJustify"                  , mimas::AlignJustify},
  {"AlignTop"                      , mimas::AlignTop},
  {"AlignBottom"                   , mimas::AlignBottom},
  {"AlignVCenter"                  , mimas::AlignVCenter},
  {"AlignCenter"                   , mimas::AlignCenter},
  {"TextDontClip"                  , mimas::TextDontClip},
  {"TextSingleLine"                , mimas::TextSingleLine},
  {"TextExpandTabs"                , mimas::TextExpandTabs},
  {"TextShowMnemonic"              , mimas::TextShowMnemonic},
  {"TextWordWrap"                  , mimas::TextWordWrap},
  {"TextIncludeTrailingSpaces"     , mimas::TextIncludeTrailingSpaces},
  {"Fixed"                         , mimas::Fixed},
  {"Minimum"                       , mimas::Minimum},
  {"Maximum"                       , mimas::Maximum},
  {"Preferred"                     , mimas::Preferred},
  {"Expanding"                     , mimas::Expanding},
  {"MinimumExpanding"              , mimas::MinimumExpanding},
  {"Ignored"                       , mimas::Ignored},
  {"NoButton"                      , mimas::NoButton},
  {"LeftButton"                    , mimas::LeftButton},
  {"RightButton"                   , mimas::RightButton},
  {"MidButton"                     , mimas::MidButton},
  {"MiddleButton"                  , mimas::MiddleButton},
  {"XButton1"                      , mimas::XButton1},
  {"XButton2"                      , mimas::XButton2},
  {"MousePress"                    , mimas::MousePress},
  {"MouseRelease"                  , mimas::MouseRelease},
  {"DoubleClick"                   , mimas::DoubleClick},
  {"NoModifier"                    , mimas::NoModifier},
  {"ShiftModifier"                 , mimas::ShiftModifier},
  {"ControlModifier"               , mimas::ControlModifier},
  {"AltModifier"                   , mimas::AltModifier},
  {"MetaModifier"                  , mimas::MetaModifier},
  {"KeypadModifier"                , mimas::KeypadModifier},
  {"GroupSwitchModifier"           , mimas::GroupSwitchModifier},
  {"Horizontal"                    , mimas::Horizontal},
  {"Vertical"                      , mimas::Vertical},
  {NULL, NULL},
};


#ifdef DUB_LUA_NO_OPEN
int luaload_mimas_constants(lua_State *L) {
#else
extern "C" int luaopen_mimas_constants(lua_State *L) {
#endif
  // register functions
  luaL_register(L, "mimas", mimas_functions);


  // register namespace enums
  register_constants(L, "mimas", mimas_namespace_constants);

  return 0;
}

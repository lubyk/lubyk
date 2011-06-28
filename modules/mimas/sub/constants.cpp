#include "mimas/constants.h"

#include "lua_cpp_helper.h"

using namespace mimas;



/** void mimas::setHue(QPalette &palette, float hue)
 * include/mimas/mimas.h:43
 */
static int mimas_setHue(lua_State *L) {
  try {
    QPalette *palette = *((QPalette **)luaL_checkudata(L, 1, "mimas.QPalette"));
    float hue = luaL_checknumber(L, 2);
    setHue(*palette, hue);
    return 0;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.setHue: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.setHue: Unknown exception");
  }
}



/** QVariant mimas::variantFromLua(lua_State *L, int index)
 * include/mimas/mimas.h:44
 */
static int mimas_variantFromLua(lua_State *L) {
  try {
    
    int index = luaL_checkint(L, 2);
    QVariant  retval__ = variantFromLua(L, index);
    lua_pushclass<QVariant>(L, retval__, "mimas.QVariant");
    return 1;
  } catch (std::exception &e) {
    return luaL_error(L, "mimas.variantFromLua: %s", e.what());
  } catch (...) {
    return luaL_error(L, "mimas.variantFromLua: Unknown exception");
  }
}



// Register namespace

static const struct luaL_Reg mimas_functions[] = {

  {"setHue"                        , mimas_setHue},
  {"variantFromLua"                , mimas_variantFromLua},

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

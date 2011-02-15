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



// Register namespace

static const struct luaL_Reg mimas_functions[] = {

  {"setHue"                        , mimas_setHue},

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

--[[------------------------------------------------------

  mimas
  -----

  Portable GUI scripted in Lua and compatible with Lubyk.

--]]------------------------------------------------------
require 'mimas.core'
mimas_core = mimas
mimas = Autoload('mimas')
-- copy constants
for k, v in pairs(mimas_core) do
  if type(v) == 'number' then
    mimas[k] = v
  end
end

-- no need for a custom loader for these
mimas.Brush   = mimas_core.Brush
mimas.Color   = mimas_core.Color
mimas.Pen     = mimas_core.Pen
mimas.Painter = mimas_core.Painter
mimas.Path    = mimas_core.Path

mimas.whitePen   = mimas.Pen(1, 0, 0, 1)
mimas.whiteBrush = mimas.Brush(0, 0, 1)
mimas.blackPen   = mimas.Pen(1, 0, 0, 0)
mimas.blackBrush = mimas.Brush(0, 0, 0)

mimas.noPen   = mimas.Pen()
mimas.noBrush = mimas.Brush()

mimas.colors = {
  black     = mimas.Color(0, 0, 0),
  lightGrey = mimas.Color(0, 0, 0.75),
  grey      = mimas.Color(0, 0, 0.5),
  darkGrey  = mimas.Color(0, 0, 0.25),
  white     = mimas.Color(0, 0, 1),
  red       = mimas.Color(0   / 360),
  green     = mimas.Color(120 / 360),
  orange    = mimas.Color(33  / 360),
  blue      = mimas.Color(240 / 360),
  pink      = mimas.Color(300 / 360),
  purple    = mimas.Color(275 / 360),
}

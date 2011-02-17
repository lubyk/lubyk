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

mimas.whiteColor = mimas.Color(0, 0, 1)
mimas.whitePen   = mimas.Pen(1, 0, 0, 1)

mimas.blackColor = mimas.Color(0, 0, 0)
mimas.blackPen   = mimas.Pen(1, 0, 0, 0)

mimas.noPen   = mimas.Pen()
mimas.noBrush = mimas.Brush()
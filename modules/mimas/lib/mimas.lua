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

--=============================================== app
local singleApp = mimas.Application()
app = singleApp
function mimas.Application()
  return singleApp
end
--=============================================== alter scheduler

-- no need for a custom loader for these
mimas.Brush   = mimas_core.Brush
mimas.Color   = mimas_core.Color
mimas.Pen     = mimas_core.Pen
mimas.Painter = mimas_core.Painter
mimas.Path    = mimas_core.Path
mimas.Region  = mimas_core.Region

mimas.WhitePen   = mimas.Pen(1, 0, 0, 1)
mimas.WhiteBrush = mimas.Brush(0, 0, 1)
mimas.BlackPen   = mimas.Pen(1, 0, 0, 0)
mimas.BlackBrush = mimas.Brush(0, 0, 0)

mimas.EmptyPen   = mimas.Pen()
mimas.EmptyBrush = mimas.Brush()

mimas.colors = {
  Black     = mimas.Color(0, 0, 0),
  LightGrey = mimas.Color(0, 0, 0.75),
  Grey      = mimas.Color(0, 0, 0.5),
  DarkGrey  = mimas.Color(0, 0, 0.25),
  White     = mimas.Color(0, 0, 1),
  Red       = mimas.Color(0   / 360),
  Orange    = mimas.Color(33  / 360),
  Yellow    = mimas.Color(60  / 360),
  Green     = mimas.Color(120 / 360),
  Blue      = mimas.Color(240 / 360),
  Pink      = mimas.Color(300 / 360),
  Purple    = mimas.Color(275 / 360),
}

mimas.ESC   = 16777216
mimas.Space = 32

sched:mimasLoaded()

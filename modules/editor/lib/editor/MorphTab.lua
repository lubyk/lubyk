--[[------------------------------------------------------

  editor.ProcessTab
  -----------------

  Representation of the morph process in the machine list
  view.

  The MorphTab is shown with a special sign and does
  nothing for the moment.

--]]------------------------------------------------------
local lib = lk.SubClass(mimas, 'Widget')
editor.MorphTab = lib

-- CONSTANTS
local HPEN_WIDTH = 1

function lib:init(morph)
  self.morph = morph
  morph.tab  = self
  self.name  = morph.name
  self:setTitle(morph.title)
  self:setHue(0)
end

lib.setTitle     = editor.ProcessTab.setTitle
lib.paint        = editor.ProcessTab.paint
lib.setAlignment = editor.ProcessTab.setAlignment

function lib:setHue(hue)
  local color
  if self.morph.online then
    color = mimas.Color(0.3, 0.3, 1)
  else
    color = mimas.Color(0, 1, 1)
  end
  self.text_pen = mimas.Pen(1, color)
  self.pen = mimas.Pen(HPEN_WIDTH * 2, color:colorWithAlpha(0.5))
  self.brush = mimas.Brush(mimas.Color(0, 0, 0.3, 0.5))

  self:update()
end

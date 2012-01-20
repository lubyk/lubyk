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
  self.morph = service
  self.name  = morph.name
  self:setTitle(morph.title)

  self.pen   = mimas.Pen(HPEN_WIDTH * 2, mimas.Color(0, 0, 0.8, 0.5))
  self.brush = mimas.Brush(mimas.Color(0, 0, 0.3, 0.5))
end

lib.setTitle     = editor.ProcessTab.setTitle
lib.paint        = editor.ProcessTab.paint
lib.setAlignment = editor.ProcessTab.setAlignment

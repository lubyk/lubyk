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
local private = {}

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

local       RightButton,       MousePress,       MetaModifier =
      mimas.RightButton, mimas.MousePress, mimas.MetaModifier

function lib:click(x, y, op, btn, mod)
  if op == MousePress and
     (btn == RightButton or mod == MetaModifier) then
    local sx, sy = self:globalPosition()
    private.showContextMenu(self, sx + x, sy + y)
  end
end

function private:showContextMenu(gx, gy)
  local menu = mimas.Menu('')
  if self.menu and not menu:deleted() then
    self.menu:hide()
  end
  self.menu = menu

  menu:addAction('Stop', '', function()
    self.morph:quit()
  end)
  menu:popup(gx - 5, gy - 5)
end

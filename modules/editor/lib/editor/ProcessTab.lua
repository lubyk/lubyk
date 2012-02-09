--[[------------------------------------------------------

  editor.ProcessTab
  -----------------

  Small tab on the right side of the PatchView or left
  side of the ControlView.

  The ProcessTab contains information on the remote
  Process such as:
   - name
   - alerts

--]]------------------------------------------------------
local lib = lk.SubClass(mimas, 'Widget')
editor.ProcessTab = lib

-- constants
local private = {}
local BOX_PADDING = 1
local HPEN_WIDTH = 1 -- half pen width
local bp = HPEN_WIDTH + BOX_PADDING -- full box padding
local arc_radius = 8
local TEXT_PADDING = 5
local CENTER_TEXT_PADDING = 9

function lib:init(process)
  self.process  = process
  process.tab   = self
  self.name     = process.name
  self:setTitle(process.name)
  self:setHue(process.hue)
end

function lib:setHue(hue)
  if self.process.online then
    self.pen   = mimas.Pen(HPEN_WIDTH * 2, mimas.Color(self.process.hue, 0.3, 0.8, 0.8))
    self.brush = mimas.Brush(mimas.Color(self.process.hue, 0.3, 0.3, 0.8))
  else
    self.pen   = mimas.Pen(HPEN_WIDTH * 2, mimas.Color(0, 0, 0.8, 0.5))
    self.brush = mimas.Brush(mimas.Color(0, 0, 0.3, 0.5))
  end

  self:update()
end

function lib:setTitle(name)
  self.title = name
  local w, h = self.super:textSize(name)
  if self.align == mimas.AlignCenter then
    self.min_width = w + 4 * CENTER_TEXT_PADDING -- 2 paddings on sides
  else
    self.min_width = w + 4 * TEXT_PADDING -- 4 paddings on sides
  end
  self.min_height = h + 2 * TEXT_PADDING
  self:setSizeHint(self.min_width, self.min_height)
  self:setSizePolicy(mimas.Minimum, mimas.Fixed)
end

function lib:paint(p, w, h)
  p:setBrush(self.brush)
  p:setPen(self.pen)
  if self.align == mimas.AlignRight then
    p:drawRoundedRect(bp, bp, w + arc_radius + bp, h - 2 * bp, arc_radius)
    p:setPen(mimas.Pen(1, mimas.Color(0, 0, 1)))
    p:drawText(2 * TEXT_PADDING, TEXT_PADDING, w - 4*TEXT_PADDING, h - 2*TEXT_PADDING, mimas.AlignRight + mimas.AlignVCenter, self.title)
  elseif self.align == mimas.AlignLeft then
    p:drawRoundedRect(-bp-arc_radius, bp, w + arc_radius, h - 2 * bp, arc_radius)
    p:setPen(mimas.Pen(1, mimas.Color(0, 0, 1)))
    p:drawText(2 * TEXT_PADDING, TEXT_PADDING, w - 4*TEXT_PADDING, h - 2*TEXT_PADDING, mimas.AlignLeft + mimas.AlignVCenter, self.title)
  else
    -- center
    p:drawRoundedRect(bp, bp, w - 2 * bp, h - 2 * bp, arc_radius)
    p:setPen(mimas.Pen(1, mimas.Color(0, 0, 1)))
    p:drawText(2 * TEXT_PADDING, TEXT_PADDING, w - 4*TEXT_PADDING, h - 2*TEXT_PADDING, mimas.AlignCenter + mimas.AlignVCenter, self.title)
  end
end

local MousePress, MouseRelease = mimas.MousePress, mimas.MouseRelease

function lib:click(x, y, op, btn, mod)
  if not self.process.online then
    return
  end
  if op == MousePress then
    local sx, sy = self:globalPosition()
    private.showContextMenu(self, sx + x, sy + y)
  end
end

function private:showContextMenu(gx, gy)
  if not self.process.online then
    return false
  end

  local menu = mimas.Menu('')
  if self.menu and not menu:deleted() then
    self.menu:hide()
  end
  self.menu = menu

  local show_hide
  if self.process.view then
    show_hide = 'Hide'
  else
    show_hide = 'Show'
  end
  menu:addAction(show_hide, '', function()
    self.process:toggleView()
  end)

  menu:addAction('Restart', '', function()
    self.process:restart()
  end)

  menu:addAction('Remove', '', function()
    self.process:remove()
  end)
  menu:popup(gx - 5, gy - 5)
end

function lib:setAlignment(align)
  self.align = align
  self:update()
  -- update SizeHint
  self:setTitle(self.title)
end

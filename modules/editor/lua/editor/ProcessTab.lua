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
local lib = mimas.WidgetClass()
editor.ProcessTab = lib

-- constants
local box_padding = 1
local hpen_width = 1 -- half pen width
local bp = hpen_width + box_padding -- full box padding
local arc_radius = 8
local text_padding = 5

function lib:init(process)
  self.process = process
  self:setName(process.name)
end

function lib:setName(name)
  self.name  = name
  local w, h = self.super:textSize(name)
  self.width  = w + 4 * text_padding -- 2 paddings on sides
  self.height = h + 2 * text_padding
  self:resize(self.width, self.height)
end

-- custom paint
function lib:paint(p, w, h)
  local pen   = mimas.Pen(hpen_width * 2, mimas.Color(self.process.hue, 0.3, 0.8, 0.8))
  local brush = mimas.Brush(mimas.Color(self.process.hue, 0.3, 0.3, 0.8))
  p:setBrush(brush)
  p:setPen(pen)
  p:drawRoundedRect(bp, bp, w + arc_radius + bp, h - 2 * bp, arc_radius)
  p:setPen(mimas.Pen(1, mimas.Color(0, 0, 1)))
  p:drawText(2 * text_padding, text_padding, w - 4*text_padding, h - 2*text_padding, mimas.AlignRight + mimas.AlignVCenter, self.name)
end

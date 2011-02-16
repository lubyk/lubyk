--[[------------------------------------------------------

  editor.ProcessView
  ------------------

  The ProcessView is where the patch for a Process is
  displayed and edited.

--]]------------------------------------------------------
local lib = mimas.WidgetClass()
editor.ProcessView = lib

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
  self.lbl_w = w + 4 * text_padding -- 2 paddings on sides
  self.lbl_h = h + 2 * text_padding
  app:post(function()
    self:update()
  end)
end

-- custom paint
function lib:paint(p, w, h)
  local pen   = mimas.Pen(hpen_width * 2, mimas.Color(self.process.hue, 0.3, 0.8, 0.8))
  local brush = mimas.Brush(self.process.hue, 0.5, 0.5, 0.5)

  -- label width/height
  local lw = self.lbl_w
  local lh = self.lbl_h
  local path = mimas.Path()

  -- draw rounded surface
  p:setBrush(self.process.hue, 0.2, 0.2) -- dark background
  p:setPen(pen)
  p:drawRoundedRect(bp, bp, w - 2 * bp, h - 2 * bp, arc_radius / 2)

  -- draw label background
  p:setPen(mimas.Pen()) -- no pen
  p:setBrush(brush)

  local pad  = bp + hpen_width -- padding for inner shape (top/left)
  path:moveTo(lw+pad, pad)
  path:lineTo(pad + arc_radius, pad)
  path:cubicTo(pad-1, pad-1, pad-1, pad-1, pad, pad + arc_radius)
  path:lineTo(pad, lh + pad)
  path:lineTo(lw - arc_radius + pad, lh + pad)
  path:cubicTo(lw+pad+1, lh+pad+1, lw+pad+1, lh+pad+1, lw+pad, lh+pad-arc_radius)
  path:lineTo(lw+pad, pad)
  p:drawPath(path)

  -- draw label border (only bottom and right)
  p:setBrush(mimas.Brush())
  p:setPen(pen)
  local pad2 = bp + 2*hpen_width -- do not overlap with background
  path = mimas.Path()
  path:moveTo(pad2, lh + pad2)
  path:lineTo(lw - arc_radius + pad2, lh + pad2)
  path:cubicTo(lw+pad2, lh+pad2, lw+pad2, lh+pad2, lw+pad2, lh+pad2-arc_radius)
  path:lineTo(lw+pad2, pad2)
  p:drawPath(path)

  -- draw label text
  p:setPen(mimas.Pen(1, mimas.Color(0, 0, 1)))
  p:drawText(pad+2*text_padding, pad+text_padding, lw - 4*text_padding, lh - 2*text_padding, mimas.AlignRight + mimas.AlignVCenter, self.name)
end

--[[------------------------------------------------------

  editor.NodeView
  ---------------

  The NodeView show a single node with inlets and outlets.

--]]------------------------------------------------------
local lib = mimas.WidgetClass()
editor.NodeView = lib

-- constants
local box_padding = 1
local hpen_width = 1 -- half pen width
local bp = hpen_width + box_padding -- full box padding
local arc_radius = 0
local text_hpadding = 10
local text_vpadding = 6
local pad  = bp + hpen_width -- padding for inner shape (top/left)
local slotw = 9
local sloth = 5
local slot_padding = 14 -- space between slots

function lib:init(node)
  self.node = node
  self:setName(node.name)
end

function lib:setName(name)
  self.name  = name
  local w, h = self.super:textSize(name)
  self.lbl_w = w + 2 * text_hpadding + 2*pad
  self.lbl_h = h + 2 * text_vpadding + 2*pad
  self:setSizePolicy(mimas.Minimum, mimas.Fixed)
  self:setSizeHint(self.lbl_w, self.lbl_h)
  app:post(function()
    self:update()
  end)
end

local function draw_slots(list, p, w, h, x, y, color)
  p:setPen(mimas.Pen())
  p:setBrush(color)
  for i, slot in ipairs(list) do
    p:drawRect(x, y, slotw, sloth)
    x = x + slotw + slot_padding
    if x > w then
      return
    end
  end
end

-- custom paint
function lib:paint(p, w, h)
  local border_color = mimas.Color(self.node.hue, 0.3, 0.8, 0.8)
  local pen   = mimas.Pen(hpen_width * 2, border_color)
  local brush = mimas.Brush(self.node.hue, 0.5, 0.5, 0.5)

  -- label width/height
  local lw = self.lbl_w
  local lh = self.lbl_h
  local path = mimas.Path()

  -- draw node surface
  p:setBrush(self.node.hue, 0.2, 0.2) -- dark background
  p:setPen(pen)
  p:drawRoundedRect(bp, bp, w - 2 * bp, h - 2 * bp, arc_radius / 2)

  -- draw label text
  p:setPen(mimas.Pen(1, mimas.Color(0, 0, 1)))
  p:drawText(pad+text_hpadding, pad+text_vpadding, w-2*text_hpadding-2*pad, h - 2*text_vpadding - 2*pad, mimas.AlignLeft + mimas.AlignVCenter, self.name)

  -- draw inlets
  local icolor = border_color
  local ocolor = border_color
  local dragging = editor.main.dragging
  if dragging then
    if dragging.type == 'inlet' then
      icolor = border_color:colorWithValue(0.38)
    elseif dragging.type == 'outlet' then
      ocolor = border_color:colorWithValue(0.38)
    end
  end

  draw_slots(self.node.inlets, p, w, h, pad+text_hpadding, pad, icolor)
  -- draw outlets
  draw_slots(self.node.outlets, p, w, h, pad+text_hpadding, h - pad - sloth, ocolor)
end

--[[------------------------------------------------------

  editor.NodeView
  ---------------

  The NodeView show a single node with inlets and outlets.

--]]------------------------------------------------------
local lib = mimas.WidgetClass()
editor.NodeView = lib

--============================================= PRIVATE
-- constants
local box_padding = 1
local hpen_width = 1 -- half pen width
local bp = hpen_width + box_padding -- full box padding
local arc_radius = 0
local text_hpadding = 10
local text_vpadding = 6
local pad  = bp + hpen_width -- padding for inner shape (top/left)
local sloth        = editor.SlotView.sloth
local slotw        = editor.SlotView.slotw
local slot_padding = editor.SlotView.slot_padding

local function makeSlotViews(self, list, type)
  for _, slot in ipairs(list) do
    -- create views for each slot
    slot.node = self.node
    slot.type = type
    slot.view = editor.SlotView(slot)
    self.super:addWidget(slot.view)
  end
end

local function placeElements(self)
  local x = pad+text_hpadding
  local y = pad
  for _, slot in ipairs(self.node.inlets) do
    slot.view:move(x, y)
    x = x + slotw + slot_padding
  end

  x = pad+text_hpadding
  y = self.height - pad - sloth
  for _, slot in ipairs(self.node.outlets) do
    slot.view:move(x, y)
    x = x + slotw + slot_padding
  end
end

--============================================= PUBLIC
function lib:init(node)
  node.view = self
  self.node = node
  self:setName(node.name)
  makeSlotViews(self, self.node.inlets, 'inlet')
  makeSlotViews(self, self.node.outlets, 'outlet')
  placeElements(self)
end

function lib:setName(name)
  self.name  = name
  local w, h = self.super:textSize(name)
  self.width = w + 2 * text_hpadding + 2*pad
  self.height = h + 2 * text_vpadding + 2*pad
  self:setSizePolicy(mimas.Minimum, mimas.Fixed)
  self:setSizeHint(self.width, self.height)
  app:post(function()
    self:update()
  end)
end

function lib:addInlet(inlet)
  addSlot(self.node.inlets, inlet)
  placeElements(self)
end

function lib:removeInlet(inlet_name)
  removeSlot(self.node.inlets, inlet_name)
  placeElements(self)
end

function lib:addOutlet(outlet)
  addSlot(self.node.outlets, outlet)
  placeElements(self)
end

function lib:removeOutlet(outlet_name)
  removeSlot(self.node.outlets, outlet_name)
  placeElements(self)
end

function lib:resized(w, h)
  self.width  = w
  self.height = h
  placeElements(self)
end

-- custom paint
function lib:paint(p, w, h)
  -- label width/height
  local path = mimas.Path()

  -- draw node surface
  p:setBrush(self.node.bg_color) -- dark background
  p:setPen(hpen_width * 2, self.node.color)
  p:drawRoundedRect(bp, bp, w - 2 * bp, h - 2 * bp, arc_radius / 2)

  -- draw label text
  p:setPen(mimas.whitePen)
  p:drawText(pad+text_hpadding, pad+text_vpadding, w-2*text_hpadding-2*pad, h - 2*text_vpadding - 2*pad, mimas.AlignLeft + mimas.AlignVCenter, self.name)
end

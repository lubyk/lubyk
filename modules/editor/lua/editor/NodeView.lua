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
local text_vpadding = 4
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

local function placeSlots(slot_list, x, y, max_x)
  for _, slot in ipairs(slot_list) do
    if slot.view then
      if x > max_x then
        slot.view:hide()
      else
        slot.view:show()
      end
      slot.view:move(x, y)
    end
    x = x + slotw + slot_padding
  end
end

local function placeElements(self)
  -- inlets
  placeSlots(self.node.inlets,
    -- start x
    pad + text_hpadding,
    -- start y
    pad,
    -- max x
    self.width - slotw - pad
  )

  placeSlots(self.node.outlets,
    -- start x
    pad + text_hpadding,
    -- start y
    self.height - pad - sloth,
    -- max x
    self.width - slotw - pad
  )
end

--============================================= PUBLIC
function lib:init(node, parent_view)
  node.view = self
  self.node = node
  self:setName(node.name)
  if parent_view then
    parent_view:addWidget(self)
    self:resize(self.width, self.height)
  end
  self:move(node.x, node.y)
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

--[[------------------------------------------------------

  editor.NodeView
  ---------------

  The NodeView show a single node with inlets and outlets.

--]]------------------------------------------------------
local lib = mimas.WidgetClass()
editor.NodeView = lib

--============================================= PRIVATE
-- constants
local BOX_PADDING = 1
local HPEN_WIDTH  = 1 -- half pen width
local BP = HPEN_WIDTH + BOX_PADDING -- full box padding
local ARC_RADIUS = 0
local TEXT_HPADDING = 10
local TEXT_VPADDING = 2
local PAD  = BP + HPEN_WIDTH -- padding for inner shape (top/left)
local SLOTH        = editor.SlotView.SLOTH
local SLOTW        = editor.SlotView.SLOTW
local SLOT_PADDING = editor.SlotView.SLOT_PADDING
local MINW         = 60
local GHOST_ALPHA  = 0.3

local function updateSlotViews(self, list, type)
  for _, slot in pairs(list) do
    -- create/update views for each slot
    slot:updateView(self)
    --slot.node = self.node
    --slot.type = type
    --slot.view = editor.SlotView(slot)
    --self.super:addWidget(slot.view)
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
      slot:updateLinkViews()
    end
    x = x + SLOTW + SLOT_PADDING
  end
end

local function placeElements(self)
  -- inlets
  placeSlots(self.node.sorted_inlets,
    -- start x
    PAD + TEXT_HPADDING,
    -- start y
    PAD,
    -- max x
    self.width - SLOTW - PAD
  )

  placeSlots(self.node.sorted_outlets,
    -- start x
    PAD + TEXT_HPADDING,
    -- start y
    self.height - PAD - SLOTH,
    -- max x
    self.width - SLOTW - PAD
  )
end

--============================================= PUBLIC
function lib:init(node, parent_view)
  self.node = node
  self:setName(node.name)
  if parent_view then
    parent_view:addWidget(self)
    self:resize(self.width, self.height)
  end
end

function lib:updateView()
  local node = self.node
  self:move(node.x, node.y)
  updateSlotViews(self, node.inlets, 'inlet')
  updateSlotViews(self, node.outlets, 'outlet')
  placeElements(self)
  -- forces redraw
  self:update()
end

function lib:setName(name)
  self.name  = name
  local w, h = self.super:textSize(name)
  self.width = math.max(w + 2 * TEXT_HPADDING + 2*PAD, MINW)
  self.height = h + 2 * TEXT_VPADDING + 2*PAD
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
  if self.is_ghost then
    p:setBrush(self.node.bg_color:colorWithAlpha(GHOST_ALPHA)) -- dark background
    p:setPen(HPEN_WIDTH * 2, self.node.color:colorWithAlpha(GHOST_ALPHA))
  else
    p:setBrush(self.node.bg_color) -- dark background
    p:setPen(HPEN_WIDTH * 2, self.node.color)
  end
  p:drawRoundedRect(BP, BP, w - 2 * BP, h - 2 * BP, ARC_RADIUS / 2)

  -- draw label text
  if self.is_ghost then
    p:setPen(1, mimas.Color(0, 0, 1, GHOST_ALPHA))
  else
    p:setPen(mimas.whitePen)
  end
  p:drawText(PAD+TEXT_HPADDING, PAD+TEXT_VPADDING, w-2*TEXT_HPADDING-2*PAD, h - 2*TEXT_VPADDING - 2*PAD, mimas.AlignLeft + mimas.AlignVCenter, self.name)
end

local MousePress, MouseRelease = mimas.MousePress, mimas.MouseRelease

local function makeGhost(self)
  local node = self.node
  node.ghost = self
  self.is_ghost = true
  -- copy
  node.view = editor.NodeView(node, node.process.view)
  self:raise()
  -- duplicate links
  node.view:updateView()
end

function lib:click(x, y, type)
  local node = self.node
  if type == MousePress then
    -- start drag operation: self becomes ghost
    self.node.dragging = true
    self.click_position = {x = x, y = y}
    self.current_pos    = {x = node.x, y = node.y}
    makeGhost(self)
  elseif type == MouseRelease then
    -- drop
    self.node.dragging = false
    app:post(function()
      node:set {
        x = self.current_pos.x,
        y = self.current_pos.y,
      }
    end)
  end
end


function lib:mouse(x, y)
  local node = self.node
  if self.is_ghost then
    local x = self.current_pos.x + x - self.click_position.x
    local y = self.current_pos.y + y - self.click_position.y
    self:move(
      x,
      y
    )
    self.current_pos = {x = x, y = y}
    -- Forces link redraw
    updateSlotViews(self, node.inlets, 'inlet')
    updateSlotViews(self, node.outlets, 'outlet')
  end
end
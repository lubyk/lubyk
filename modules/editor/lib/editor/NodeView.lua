--[[------------------------------------------------------

  editor.NodeView
  ---------------

  The NodeView show a single node with inlets and outlets.

--]]------------------------------------------------------
local lib = lk.SubClass(mimas, 'Widget')
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
local SELECTED_COLOR_VALUE = 0.6
local START_DRAG_DIST = 4

local function updateSlotViews(self, list, type)
  for _, slot in ipairs(list) do
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
  self.delegate = node.delegate
  self:setName(node.name)
  if parent_view then
    -- If we do not cache these, they endup wrong (resized callback?)
    local w, h = self.width, self.height
    parent_view:addWidget(self)
    self:resize(w, h)
  else
    print('Start NodeView without parent')
  end
end

function lib:updateView()
  local node = self.node
  self:move(node.x, node.y)
  updateSlotViews(self, node.sorted_inlets, 'inlet')
  updateSlotViews(self, node.sorted_outlets, 'outlet')
  placeElements(self)
  -- forces redraw
  self:update()
end

function lib:setName(name)
  self.name  = name
  local w, h = self.super:textSize(name)
  self.width = math.max(w + 2 * TEXT_HPADDING + 2*PAD, MINW)
  self.height = h + 2 * TEXT_VPADDING + 2*PAD
  self:setSizeHint(self.width, self.height)
  self:setSizePolicy(mimas.Minimum, mimas.Fixed)
  self:update()
end

-- function lib:addInlet(inlet)
--   addSlot(self.node.inlets, inlet)
--   placeElements(self)
-- end
--
-- function lib:removeInlet(inlet_name)
--   removeSlot(self.node.inlets, inlet_name)
--   placeElements(self)
-- end
--
-- function lib:addOutlet(outlet)
--   addSlot(self.node.outlets, outlet)
--   placeElements(self)
-- end
--
-- function lib:removeOutlet(outlet_name)
--   removeSlot(self.node.outlets, outlet_name)
--   placeElements(self)
-- end

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
    p:setBrush(self.node.bg_color:colorWithAlpha(GHOST_ALPHA))
    p:setPen(HPEN_WIDTH * 2, self.node.color:colorWithAlpha(GHOST_ALPHA))
  elseif self.selected then
    p:setBrush(self.node.bg_color:colorWithValue(SELECTED_COLOR_VALUE))
    p:setPen(HPEN_WIDTH * 2, self.node.color)
  else
    p:setBrush(self.node.bg_color)
    p:setPen(HPEN_WIDTH * 2, self.node.color)
  end
  p:drawRoundedRect(BP, BP, w - 2 * BP, h - 2 * BP, ARC_RADIUS / 2)

  -- draw label text
  if self.is_ghost then
    p:setPen(1, mimas.Color(0, 0, 1, GHOST_ALPHA))
  else
    p:setPen(mimas.WhitePen)
  end
  p:drawText(PAD+TEXT_HPADDING, PAD+TEXT_VPADDING, w-2*TEXT_HPADDING-2*PAD, h - 2*TEXT_VPADDING - 2*PAD, mimas.AlignLeft + mimas.AlignVCenter, self.name)
end

local MousePress,       MouseRelease,       DoubleClick =
      mimas.MousePress, mimas.MouseRelease, mimas.DoubleClick

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

function lib:click(x, y, type, btn, mod)
  local node = self.node
  if type == MousePress then
    -- store position but only start drag when moved START_DRAG_DIST away
    self.click_position = {x = x, y = y}
    self.current_pos    = {x = node.x, y = node.y}
  elseif type == DoubleClick then
    -- open external editor
    node:edit()
    self.delegate:selectNodeView(self)
  elseif type == MouseRelease then
    if node.dragging then
      -- drop
      node.dragging = false
      node:change {
        x = self.current_pos.x,
        y = self.current_pos.y,
      }
    else
      self.delegate:selectNodeView(self, mod == mimas.ShiftModifier)
    end
  end
end

local function manhattanDist(a, b)
  return math.abs(b.x - a.x) + math.abs(b.y - a.y)
end

function lib:mouse(x, y)
  local node = self.node
  if not node.dragging and manhattanDist(self.click_position, {x=x,y=y}) > START_DRAG_DIST then
    -- start drag operation: self becomes ghost
    node.dragging = true
    makeGhost(self)
  end

  if self.is_ghost then
    local x = self.current_pos.x + x - self.click_position.x
    local y = self.current_pos.y + y - self.click_position.y
    self:move(x, y)
    self.current_pos = {x = x, y = y}
    -- Forces link redraw
    updateSlotViews(self, node.sorted_inlets, 'inlet')
    updateSlotViews(self, node.sorted_outlets, 'outlet')
  end
end

function lib:delete()
  self:hide()
  if self.edit then
    self.edit:hide()
  end
  if self.selected then
    -- remove ghost from selection my selecting only self
    self.delegate:selectNodeView(self)
  end
  self.super:__gc()
end

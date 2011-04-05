--[[------------------------------------------------------

  editor.SlotView
  ---------------

  The SlotView show a single slot (inlet or outlet).

--]]------------------------------------------------------
local lib = mimas.WidgetClass()
editor.SlotView = lib

-- constants
local box_padding = 1
local hpen_width = 1 -- half pen width
local bp = hpen_width + box_padding -- full box padding
local arc_radius = 0
local text_hpadding = 10
local text_vpadding = 6
local pad  = bp + hpen_width -- padding for inner shape (top/left)
local SLOTW = 7
local SLOTH = 4
local SLOT_PADDING = 10 -- space between slots
local START_DRAG_DIST = 4
local LINK_DISTANCE = 100

-- Needed by NodeView and LinkView
lib.SLOTW = SLOTW
lib.SLOTH = SLOTH
lib.SLOT_PADDING = SLOT_PADDING

function lib:init(slot)
  self.type = slot.type
  self.slot = slot
  self.node = slot.node
  self.delegate = slot.node.delegate
  self:resize(SLOTW, SLOTH)
end

-- custom paint
function lib:paint(p, w, h)
--  p:setPen(mimas.noPen)

  -- draw inlets
  local color

  if self.delegate.closest_slot_view == self then
    color = self.node.color:colorWithValue(1)
  else
    color = self.node.color:colorWithValue(0.5)
  end
  p:fillRect(0, 0, w, h, color)
end

local MousePress,       MouseRelease,       DoubleClick =
      mimas.MousePress, mimas.MouseRelease, mimas.DoubleClick

function lib:click(x, y, type, btn, mod)
  local slot = self.slot
  if type == MousePress then
    -- store position but only start drag when moved START_DRAG_DIST away
    self.click_position = {x = x, y = y}
  elseif type == MouseRelease then
    if slot.dragging then
      local other_view = self.delegate.closest_slot_view
      -- create link
      slot.dragging = false
      if other_view then
        local other_slot = other_view.slot
        self.delegate.closest_slot_view = nil
        other_view:update()
        self.ghost.link_view:delete()
        self.ghost = nil
        if slot.type == 'editor.Inlet' then
          slot, other_slot = other_slot, slot
        end
        slot.node:change {
          links = {
            [slot.name] = {
              [other_slot:url()] = true
            }
          }
        }
      end
    end
  end
end

local function manhattanDist(a, b)
  return math.abs(b.x - a.x) + math.abs(b.y - a.y)
end

local function makeGhostLink(self)
  local slot = self.slot
  self.ghost = {
    slot_view = editor.SlotView(slot)
  }
  if slot.type == 'editor.Inlet' then
    self.ghost.link_view = editor.LinkView(self.ghost.slot_view, self)
  else
    self.ghost.link_view = editor.LinkView(self, self.ghost.slot_view)
  end
  slot.node.process.view:addWidget(self.ghost.link_view)
  self.ghost.link_view:lower()
end

function lib:mouse(x, y)
  local slot = self.slot
  if self.click_position and not slot.dragging and manhattanDist(self.click_position, {x=x,y=y}) > START_DRAG_DIST then
    -- start drag operation: create ghost Link
    slot.dragging = true
    makeGhostLink(self)
  end

  if self.ghost then
    local gx, gy = self:globalPosition()
    gx = gx + x
    gy = gy + y
    local view, d = self.node.process.view:closestSlotView(gx, gy, self.type, slot.node)
    local old_closest = self.delegate.closest_slot_view
    if d < LINK_DISTANCE then
      self.delegate.closest_slot_view = view
      if old_closest ~= view then
        if old_closest then
          old_closest:update()
        end
        view:update()
      end
    elseif old_closest then
      self.delegate.closest_slot_view = nil
      old_closest:update()
    end
    self.ghost.slot_view:globalMove(gx - SLOTW/2, gy - SLOTH/2)
    self.ghost.link_view:slotMoved()
  end
end
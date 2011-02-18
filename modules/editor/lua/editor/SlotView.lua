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
local slotw = 9
local sloth = 5
local slot_padding = 14 -- space between slots

-- Needed by NodeView and LinkView
lib.slotw = slotw
lib.sloth = sloth
lib.slot_padding = slot_padding

function lib:init(slot)
  self.type = slot.type
  self.slot = slot
  slot.view = self
  self.node = slot.node
  self:resize(slotw, sloth)
end

-- custom paint
function lib:paint(p, w, h)
--  p:setPen(mimas.noPen)

  -- draw inlets
  local color = self.node.color

  local dragging = editor.main.dragging
  if dragging then
    if dragging.type == self.type then
      color = color:colorWithValue(0.38)
    end
  end
  p:fillRect(0, 0, w, h, color)
end

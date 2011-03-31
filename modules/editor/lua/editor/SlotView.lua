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

-- Needed by NodeView and LinkView
lib.SLOTW = SLOTW
lib.SLOTH = SLOTH
lib.SLOT_PADDING = SLOT_PADDING

function lib:init(slot)
  self.type = slot.type
  self.slot = slot
  self.node = slot.node
  self:resize(SLOTW, SLOTH)
end

-- custom paint
function lib:paint(p, w, h)
--  p:setPen(mimas.noPen)

  -- draw inlets
  local color = self.node.color

  local dragging = editor.main.dragging
  if not dragging or dragging.type == self.type then
    color = color:colorWithValue(0.5)
  end
  p:fillRect(0, 0, w, h, color)
end

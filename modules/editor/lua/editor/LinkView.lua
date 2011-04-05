--[[------------------------------------------------------

  editor.LinkView
  ---------------

  The LinkView show a single link between an outlet and an
  inlet.

--]]------------------------------------------------------
local lib = mimas.WidgetClass()
editor.LinkView = lib
-- constants
local HPEN_WIDTH = 1 -- half pen width
local SLOTW = editor.SlotView.SLOTW
local SLOTH = editor.SlotView.SLOTH
local VECV  = 80      -- force vertical start/end
local PADV  = VECV/3  -- vertical pad needed to draw up/down curve when
                         -- outlet is lower from inlet
local GHOST_ALPHA = 0.3

function lib:init(source, target)
  self.source = source
  self.target = target
  self.outlet = source.slot
  self.inlet  = target.slot
  -- cache
  self.pen = mimas.Pen(2 * HPEN_WIDTH, self.outlet.node.color)

  self:slotMoved()
end

local function makePath(x1, y1, x2, y2)
  local path = mimas.Path()
  path:moveTo(x1, y1)
  local vect = math.min(VECV, math.abs(x2 - x1)*0.3 + VECV*0.1);
  vect = math.min(vect, math.abs(y2 - y1) + VECV*0.1);

  path:cubicTo(
    x1, y1 + vect,
    x2, y2 - vect,
    x2, y2
  );
  return path
end

-- must be called whenever a slot (or a NodeView/Patch) is moved.
function lib:slotMoved()
  -- global start position
  local x1, y1 = self.source:globalPosition()
  x1 = x1 + SLOTW/2
  y1 = y1 + SLOTH
  -- global end position
  local x2, y2 = self.target:globalPosition()
  x2 = x2 + SLOTW/2
  -- global position of this widget
  local x = math.min(x1, x2) - HPEN_WIDTH
  local y = math.min(y1, y2) - PADV
  local w = math.abs(x2 - x1) + 2*HPEN_WIDTH
  local h = math.abs(y2 - y1) + 2*PADV
  self.path = makePath(x1 - x, y1 - y, x2 - x, y2 - y)
  -- own global position
  self:globalMove(x, y)
  self:resize(w, h)
  -- force redraw in case we do not move but are
  -- in the end of a drag operation (ghost looking)
  self:lower()
  self:update()
end

-- custom paint
function lib:paint(p, w, h)
  local color = self.outlet.node.color
  local dragging = editor.main.dragging
  if dragging then
    color = color:colorWithValue(0.38)
  elseif self.source.is_ghost or self.target.is_ghost then
    color = color:colorWithAlpha(GHOST_ALPHA)
  end
  p:setPen(2*HPEN_WIDTH, color)
  p:drawPath(self.path)
end

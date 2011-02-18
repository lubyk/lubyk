--[[------------------------------------------------------

  editor.LinkView
  ---------------

  The LinkView show a single link between an outlet and an
  inlet.

--]]------------------------------------------------------
local lib = mimas.WidgetClass()
editor.LinkView = lib
-- constants
local hpen_width = 1 -- half pen width
local slotw = editor.SlotView.slotw
local sloth = editor.SlotView.sloth
local vecv  = 80      -- force vertical start/end
local padv  = vecv/3  -- vertical pad needed to draw up/down curve when
                         -- outlet is lower from inlet

function lib:init(outlet_view, inlet_view)
  self.outlet = outlet_view.slot
  self.inlet  = inlet_view.slot
  -- cache
  self.pen = mimas.Pen(2 * hpen_width, self.outlet.node.color)
  self:slotMoved()
end

local function makePath(x1, y1, x2, y2)
  local path = mimas.Path()
  path:moveTo(x1, y1)
  local vect = math.min(vecv, math.abs(x2 - x1)*0.3 + vecv*0.1);
  vect = math.min(vect, math.abs(y2 - y1) + vecv*0.1);

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
  local x1, y1 = self.outlet.view:globalPosition()
  x1 = x1 + slotw/2
  y1 = y1 + sloth
  -- global end position
  local x2, y2 = self.inlet.view:globalPosition()
  x2 = x2 + slotw/2
  -- global position of this widget
  local x = math.min(x1, x2) - hpen_width
  local y = math.min(y1, y2) - padv
  local w = math.abs(x2 - x1) + 2*hpen_width
  local h = math.abs(y2 - y1) + 2*padv
  self.path = makePath(x1 - x, y1 - y, x2 - x, y2 - y)
  -- own global position
  self:globalMove(x, y)
  self:resize(w, h)
end

-- custom paint
function lib:paint(p, w, h)
  local color = self.outlet.node.color
  local dragging = editor.main.dragging
  if dragging then
    color = color:colorWithValue(0.38)
  end
  p:setPen(2*hpen_width, color)
  p:drawPath(self.path)
end

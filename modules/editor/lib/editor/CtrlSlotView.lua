--[[------------------------------------------------------

  editor.CtrlSlotView
  -------------------

  Displays a control slot during connection edit.

--]]------------------------------------------------------
local lib = lk.SubClass(mimas, 'Widget')
editor.CtrlSlotView = lib
lib.stype = 'ctrl'
local private = {}

--=============================================== CONSTANTS
local HPEN_WIDTH = 1
local DRAG_START = 20

function lib:init(conn, link_editor)
  self.link_editor = link_editor
  self.conn  = conn
  self.name  = conn.name
  self:setTitle(conn.name)

  self:setHue(conn.ctrl.hue)
  self:setAlignment(mimas.AlignRight)
end

function lib:connected()
  return self.conn.node and true
end

function lib:select(selected)
  self.selected = selected
  self:setHue(self.hue)
  self:update()
end

function lib:setHue(hue)
  if hue then
    self.hue = hue
  else
    hue = self.hue
  end

  self.pen = mimas.Pen(HPEN_WIDTH * 2, mimas.Color(hue, 0.3, 0.8, 0.8))
  self.text_pen = mimas.Pen(1, mimas.Color(0, 0, 1))

  if self.selected then
    -- selected
    self.brush = mimas.Brush(mimas.Color(hue, 0.3, 0.8, 0.8))
  elseif self:connected() then
    -- connected
    self.brush = mimas.Brush(mimas.Color(hue, 0.3, 0.3, 0.8))
  else
    -- not connected
    self.pen   = mimas.Pen(HPEN_WIDTH * 2, mimas.Color(hue, 0, 0.5, 0.8))
    self.brush = mimas.Brush(mimas.Color(hue, 0, 0.3, 0.8))
  end
end

lib.setTitle     = editor.ProcessTab.setTitle
lib.paint        = editor.ProcessTab.paint
lib.setAlignment = editor.ProcessTab.setAlignment

--=============================================== Widget callbacks

function lib:resized(w, h)
  self.w = w
  self.h = h
end

local MousePress   = mimas.MousePress
local MouseRelease = mimas.MouseRelease
local DoubleClick  = mimas.DoubleClick

function lib:click(x, y, op)
  if op == MousePress then
    self.link_editor:selectSource(self)
    self.gx, self.gy = self:globalPosition()
    self.drag = {
      x = x,
      y = y,
    }
  elseif  op == DoubleClick or
         (op == MouseRelease and not self.drag.started) then
    if self.stype == 'ctrl' and self.conn.node then
      local gx, gy = self:globalPosition()
      gx = gx + x
      gy = gy + y
      self.link_editor:editDialog(self.conn, gx, gy)
    end
  elseif op == MouseRelease then
    if self.drag.started then
      -- Create link and/or clear dragged path.
      self.link_editor:endDrag(self.stype)
    end
    self.drag = nil
  end
end

function lib:mouse(x, y)
  local drag = self.drag
  if not drag.started and
     math.abs(x - drag.x) + math.abs(y - drag.y) > DRAG_START then
    drag.started = true
  end

  if drag.started then
    local link_editor = self.link_editor
    local gx, gy = self.gx, self.gy
    x = gx + x
    y = gy + y

    link_editor:selectClosestSlot(self.stype, x, y)

    link_editor:setPath(
    self,
    x,
    y,
    'drag'
    )
  end
end

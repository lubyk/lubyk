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

  self.pen   = mimas.Pen(HPEN_WIDTH * 2, mimas.Color(hue, 0.3, 0.8, 0.8))

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

function lib:click(x, y, op)
  if op == MousePress then
    self.link_editor:selectSource(self)
    self.gx, self.gy = self:globalPosition()
  elseif op == MouseRelease then
    -- Create link and/or clear dragged path.
    self.link_editor:endDrag(self.stype)
  end
end

function lib:mouse(x, y)
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

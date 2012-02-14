--[[------------------------------------------------------

  editor.CtrlSlotView
  -------------------

  Displays a control slot during connection edit.

--]]------------------------------------------------------
local lib = lk.SubClass(mimas, 'Widget')
editor.CtrlSlotView = lib
local private = {}

--=============================================== CONSTANTS
local HPEN_WIDTH = 1
local CONNECT_DIST = 150

function lib:init(conn, ctrl_link_view)
  self.ctrl_link_view = ctrl_link_view
  self.zone  = ctrl_link_view.zone
  self.conn  = conn
  self.name  = conn.name
  self:setTitle(conn.name)

  self:setHue(conn.ctrl.hue)
  self:setAlignment(mimas.AlignRight)
end

function lib:connected()
  return self.conn.node and true
end

function lib:setHue(hue)
  if hue then
    self.hue = hue
  end

  if self:connected() then
    -- connected
    self.pen   = mimas.Pen(HPEN_WIDTH * 2, mimas.Color(hue, 0.3, 0.8, 0.8))
    self.brush = mimas.Brush(mimas.Color(hue, 0.3, 0.3, 0.8))
  else
    -- not connected
    self.pen   = mimas.Pen(HPEN_WIDTH * 2, mimas.Color(hue, 0, 0.8, 0.8))
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
    self.drag = {
      x = x,
      y = y,
    }
    local gx, gy = self:globalPosition()
    self.gx = gx
    self.gy = gy
  elseif op == MouseRelease then
    -- Create link and/or clear dragged path.
    self.ctrl_link_view:endDrag(self)
  end
end

function lib:mouse(x, y)
  local view = self.ctrl_link_view
  local gx, gy = self.gx, self.gy
  local drag = self.drag
  drag.x = gx + x
  drag.y = gy + y

  local zone = self.zone
  local node, dist = zone:closestNodeAtGlobal(self.gx + x, self.gy + y)
  -- highlight node and show connection options
  if dist < CONNECT_DIST and node then
    zone:selectNodeView(node.view)
    view:setNode(node)
  else
    zone:selectNodeView(nil)
    view:setNode(nil)
  end

  view:setPath(
    self,
    drag.x,
    drag.y,
    gx,
    gy + self.h/2
  )
end

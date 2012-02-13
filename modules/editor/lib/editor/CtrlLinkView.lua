--[[------------------------------------------------------

  editor.CtrlLinkView
  -------------------

  The CtrlLinkView show a single link between a control
  and a node.

--]]------------------------------------------------------
local lib = lk.SubClass(mimas, 'Widget')
editor.CtrlLinkView = lib

-- Box pen width
local HPEN_WIDTH = 1
-- Link pen width
local TEXT_PADDING = 3
local CONNECT_DIST = 90
local PLUG_HEIGHT = 20
local ARC_RADIUS = 8
local VECV  = 80      -- force vertical start/end
local private = {}

function lib:init(conn, zone)
  local view = zone.view
  local ctrl = conn.ctrl
  view:addWidget(self)

  self.conn = conn
  self.zone = zone
  self:setConnLabel(conn.name)
  self:setHue(ctrl.hue)

  self:move(0,0)
  local vx, vy = view:globalPosition()
  self.gx = vx
  self.gy = vy
  local cx, cy = ctrl:globalPosition()
  self.w = cx - vx
  self.h = view:height()
  self:resize(self.w, self.h)
  -- Top of control in our coordinates.
  self.cy = cy - vy
  self:setMouseTracking(true)
  self.path = mimas.Path()
  self:show()
end

function lib:setConnLabel(name)
  self.conn_lbl_w = self:textSize(name) + 4 * TEXT_PADDING + 2 * HPEN_WIDTH
  self.conn_lbl = name
end

function lib:setHue(hue)
  self.pen   = mimas.Pen(HPEN_WIDTH * 2, mimas.Color(hue, 0.3, 0.8, 0.8))
  self.brush = mimas.Brush(mimas.Color(hue, 0.3, 0.3, 0.8))
end

--=============================================== Widget callbacks

function lib:mouse(x, y)
  if self.dlg then
    return
  end
  local zone = self.zone
  private.makePath(self, x, y)
  local node, dist = zone:closestNodeAtGlobal(self.gx + x, self.gy + y)
  -- highlight node and show connection options
  if dist < CONNECT_DIST and node then
    zone:selectNodeView(node.view)
  else
    zone:selectNodeView(nil)
  end
  self:update()
end

local noBrush = mimas.EmptyBrush

function lib:click(x, y, op, btn, mod)
  if op == mimas.MousePress then
    -- Find node under click
    local node_view
    for _, view in pairs(self.zone.selected_node_views) do
      node_view = view
      break
    end
    if node_view then
      private.showContextMenu(self, node_view.node, self.gx + x, self.gy + y)
    else
      private.cleanup(self)
    end
  end
end

local noBrush = mimas.EmptyBrush

-- custom paint
function lib:paint(p, w, h)
  p:setBrush(self.brush)
  p:setPen(self.pen)
  local lbl_w = self.conn_lbl_w
  local bx, by = self.w - lbl_w, self.cy + HPEN_WIDTH
  p:drawRoundedRect(bx, by, lbl_w + ARC_RADIUS + 10, PLUG_HEIGHT, ARC_RADIUS)

  p:setPen(mimas.Pen(1, mimas.Color(0, 0, 1)))
  p:drawText(bx + 2 * TEXT_PADDING, by + TEXT_PADDING, lbl_w - 4*TEXT_PADDING, PLUG_HEIGHT - 2*TEXT_PADDING, mimas.AlignRight + mimas.AlignVCenter, self.conn.name)

  p:setBrush(noBrush)
  p:setPen(self.pen)
  p:drawPath(self.path)
end

--=============================================== PRIVATE

-- x1,y1 = mouse
function private:makePath(x1, y1)
  local x2, y2 = self.w - self.conn_lbl_w, self.cy + PLUG_HEIGHT/2
  local path = mimas.Path()
  if x1 > x2 then
    -- no path
    self.path = path
    return
  end


  path:moveTo(x1, y1)
  local vect = math.min(VECV, math.abs(y2 - y1)*0.3 + VECV*0.1);
  vect = math.min(vect, math.abs(x2 - x1) + VECV*0.1);

  path:cubicTo(
    x1 + vect, y1,
    x2 - vect, y2,
    x2, y2
  )
  self.path = path
end

-- Show node params editor menu
function private:showContextMenu(node, gx, gy)
  local menu = mimas.Menu('')
  self.node = node
  if self.menu and not menu:deleted() then
    self.menu:hide()
  end
  self.menu = menu

  local params = node.params
  local list = {}
  -- sorted params list
  for key, val in pairs(params) do
    -- Keep connectors list sorted
    local i = 1
    while list[i] and key >= list[i] do
      i = i + 1
    end
    table.insert(list, i, key)
  end

  for _, param in ipairs(list) do
    menu:addAction('link '..param, '', function()
      private.editDialog(self, param)
    end)
  end
  menu:addSeparator()
  menu:addAction('Cancel', '', function()
    private.cleanup(self)
  end)

  menu:popup(gx - 5, gy - 5)
end

-- Show edit connector dialog
function private:editDialog(param)
  local conn = self.conn
  local dlg = mimas.SimpleDialog {
    parent = self,
    flag   = mimas.WidgetFlag,
    'Edit link',
    {
      'vbox', box = true,
      'Minimal value',
      {'input', 'min', conn.min or 0},
      'Maximal value',
      {'input', 'max', conn.max or 1},
    },
    {
      'hbox', {},
      {'btn', 'Cancel'},
      {'btn', 'OK', default = true},
    },
  }

  self:addWidget(dlg)
  dlg:globalMove(self.node.view:globalPosition())
  self.dlg = dlg
  
  function dlg.btn(dlg, btn_name)
    if btn_name == 'OK' then
      -- absolute url
      local url = self.node:url() .. '/_/' .. param
      local def = {
        url = url,
        min = tonumber(dlg.form.min) or 0,
        max = tonumber(dlg.form.max) or 1,
      }
      conn.ctrl:change {
        connect = {
          [conn.name] = def,
        },
      }
    end
    private.cleanup(self)
  end
  dlg:show()
end

function private:cleanup()
  self:hide()
  self.zone:selectNodeView(nil)
  self.conn.ctrl.meta_op = nil
end

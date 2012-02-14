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
local PLUG_HEIGHT = 20
local ARC_RADIUS = 8
local VECV  = 80      -- force vertical start/end
local private = {}

function lib:init(zone)
  self.zone  = zone
  self.paths = {}
  self.slots = {}

  local view = zone.view
  view:addWidget(self)
  self:move(0,0)

  local vx, vy = view:globalPosition()
  self.gx = vx
  self.gy = vy

  self.w = view:width()
  self.h = view:height()
  self:resize(self.w, self.h)
  self.back_color = mimas.Color(0, 0, 0.1, 0.5)
  self:show()
end

function lib:setCtrl(ctrl)
  if self.slots.ctrl then
    self.slots.ctrl:hide()
  end
  if not ctrl then
    self.slots.ctrl = nil
    self.ctrl = nil
    return
  end
  self.ctrl = ctrl

  local slots = mimas.Widget()
  self.slots.ctrl = slots
  slots:setSizePolicy(mimas.Minimum, mimas.Fixed)
  self.pen  = mimas.Pen(2, mimas.Color(ctrl.hue, 0.3, 0.8, 0.8))
  slots.pen = self.pen
  function slots:paint(p, w, h)
    p:setPen(self.pen)
    p:drawLine(w - 2, 0, w - 2, h)
  end

  local vbox = mimas.VBoxLayout(slots)
  vbox:setContentsMargins(0,8,3,8)
  vbox:setSpacing(5)
  slots.vbox = vbox
  -- list of CtrlSlotViews.
  slots.list = {}

  for name, conn in ipairs(ctrl.connectors) do
    local view = editor.CtrlSlotView(conn, self)
    vbox:addWidget(view, 0, mimas.AlignRight)
    slots.list[conn] = view
    if conn.node and conn.node.view then
      -- FIXME
      -- draw path
    end
  end

  self:addWidget(slots)
  slots:show()
  local w, h = vbox:minimumSize()
  slots:resize(w, h)
  local cx, cy = ctrl:globalPosition()
  slots:globalMove(cx - w, cy)
end

function lib:setNode(node)
  if self.slots.node then
    self.slots.node:hide()
  end
  if not node then
    self.slots.node = nil
    self.node = nil
    return
  end

  self.node = node
  local slots = mimas.Widget()
  self.slots.node = slots
  slots:setSizePolicy(mimas.Minimum, mimas.Fixed)
  self.pen  = mimas.Pen(2, mimas.Color(node.hue, 0.3, 0.8, 0.8))
  slots.pen = self.pen
  function slots:paint(p, w, h)
    p:setPen(self.pen)
    p:drawLine(1, 0, 1, h)
  end

  local vbox = mimas.VBoxLayout(slots)
  vbox:setContentsMargins(2,8,0,8)
  vbox:setSpacing(5)
  slots.vbox = vbox
  -- list of ParamSlotViews.
  slots.list = {}

  local list = {}
  for key, val in pairs(node.params) do
    -- Sorted params list
    local i = 1
    while list[i] and key >= list[i] do
      i = i + 1
    end
    table.insert(list, i, key)
  end

  for _, name in ipairs(list) do
    local view = editor.ParamSlotView(name, node, self)
    vbox:addWidget(view, 0, mimas.AlignLeft)
    slots.list[name] = view
    if false then
      -- FIXME
      -- draw path
    end
  end

  self:addWidget(slots)
  slots:show()
  local w, h = vbox:minimumSize()
  slots:resize(w, h)
  local nx, ny = node.view:globalPosition()
  slots:globalMove(nx + node.view:width(), ny)
end

function lib:closestSlot(key, gx, gy)
  local list = self.slots[key]
  -- self.slots.ctrl.list
  list = list and list.list
  if list then
  end
end

function lib:endDrag(key)
  if self.node and self.ctrl then
    -- make link to closest slot
  end
  self.paths[key] = nil
  self:update()
end

function lib:setPath(key, x1, y1, x2, y2)
  local path = mimas.Path()
  x1, x2 = x1 - self.gx, x2 - self.gx
  y1, y2 = y1 - self.gy, y2 - self.gy

  path:moveTo(x1, y1)
  local vect = math.min(VECV, math.abs(y2 - y1)*0.3 + VECV*0.1);
  vect = math.min(vect, math.abs(x2 - x1) + VECV*0.1);

  path:cubicTo(
    x1 + vect, y1,
    x2 - vect, y2,
    x2, y2
  )
  self.paths[key] = path
  self:update()
end
--=============================================== Widget callbacks

local noBrush = mimas.EmptyBrush

-- Draw links ?
function lib:paint(p, w, h)
  p:fillRect(0, 0, w, h, self.back_color)
  p:setBrush(noBrush)
  p:setPen(self.pen)
  for _, path in pairs(self.paths) do
    p:drawPath(path)
  end
end

function lib:click()
  private.cleanup(self)
end
--=============================================== PRIVATE

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
      'Maximal value',
      {'input', 'max', conn.max or 1},
      'Minimal value',
      {'input', 'min', conn.min or 0},
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
  self.ctrl.meta_op = nil
end


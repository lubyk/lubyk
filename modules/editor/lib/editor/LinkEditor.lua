--[[------------------------------------------------------

  editor.LinkEditor
  -------------------

  The LinkEditor enables changing links between controls
  and nodes.

--]]------------------------------------------------------
local lib = lk.SubClass(mimas, 'Widget')
editor.LinkEditor = lib

-- Box pen width
local HPEN_WIDTH = 1
-- Link pen width
local TEXT_PADDING = 3
local PLUG_HEIGHT = 20
local ARC_RADIUS = 8
-- Slot connection distance.
local CONNECT_DIST      = 50
-- Node connection distance.
local NODE_CONNECT_DIST = 150
local VECH  = 80      -- force vertical start/end
local private = {}

function lib:init(zone)
  self.zone  = zone
  self.paths = {}
  self.slots_for = {}

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
  self.drag_pen   = mimas.Pen(2, mimas.Color(0, 0, 0.8, 0.8))
  self:show()
end

function lib:setCtrl(ctrl)
  if self.ctrl == ctrl then
    return
  end
  if not self.node then
    self.source = ctrl
  end
  if self.slots_for.param then
    self.slots_for.param:hide()
  end
  if not ctrl then
    self.slots_for.param = nil
    self.ctrl = nil
    if self.node then
      self:rebuildPaths()
    end
    return 
  end
  self.ctrl = ctrl

  local slots = mimas.Widget()
  -- Other type for 'param' is our slots.
  self.slots_for.param = slots
  slots:setSizePolicy(mimas.Minimum, mimas.Fixed)

  slots.pen  = mimas.Pen(2, mimas.Color(ctrl.hue, 0.3, 0.8, 0.8))
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
  slots:globalMove(cx - w - 1, cy)
  self:rebuildPaths()
end

function lib:setNode(node)
  if node == self.node then
    return
  end
  if not self.ctrl then
    self.source = node
  end
  if self.slots_for.ctrl then
    self.slots_for.ctrl:hide()
  end
  if not node then
    self.slots_for.ctrl = nil
    self.node = nil
    if self.ctrl then
      self:rebuildPaths()
    end
    return
  end

  self.node = node
  local slots = mimas.Widget()
  -- Other type for 'ctrl' is our list of slots.
  self.slots_for.ctrl = slots
  slots:setSizePolicy(mimas.Minimum, mimas.Fixed)
  slots.pen  = mimas.Pen(2, mimas.Color(node.hue, 0.3, 0.8, 0.8))
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
  slots:globalMove(nx + node.view:width() + 1, ny + 1)
  self:rebuildPaths()
end

function lib:rebuildPaths()
  local src = self.source
  if src then
    if self.source_slot then
      -- Remove all links except dragged path
      self.paths = {
        drag = self.paths.drag,
      }
    else
      self.paths = {}
    end

    if src.type == 'editor.Node' then
      --================================ Show all links from node
      local node = src
    else
      --================================ Show all links from Ctrl
      local ctrl  = src
      local list  = self.slots_for.param.list
      local nlist = self.slots_for.ctrl and self.slots_for.ctrl.list
      if nlist then
        for _, slot in pairs(nlist) do
          slot:setHue()
        end
      end

      for _, slot in pairs(list) do
        -- Update hue (changes on connection state) state.
        slot:setHue()
        local node = slot.conn.node
        if node and node.view then
          if node == self.node then
            -- link to slot
            local param_name = slot.conn.param_name
            local target = nlist[param_name]
            self:setPath(slot, target)
          else
            -- link to node
            local nv = node.view
            local nx, ny = nv:globalPosition()
            nx = nx + nv:width()
            ny = ny + nv:height()/2
            self:setPath(slot, nx, ny)
          end
        end
      end
    end
  else
    self.paths = {}
  end
  self:update()
end

function distanceToSlot(slot, gx, gy)
  local sx, sy = slot:globalPosition()
  sx = sx + slot:width()/2
  sy = sy + slot:height()/2
  return math.sqrt((gx-sx)^2 + (gy-sy)^2)
end

function lib:selectSource(slot)
  if self.dlg then
    self.dlg:hide()
    self.dlg = nil
  end

  if self.source_slot and self.source_slot ~= slot then
    self.source_slot:select(false)
  end
  if slot then
    slot:select(true)
    self.source = slot.conn or slot.node
  end
  self.source_slot = slot
  self:rebuildPaths()
end

function lib:selectClosestSlot(key, gx, gy)
  if key == 'ctrl' and not self.target_slot then
    -- Update the node
    local zone = self.zone
    local node, dist = zone:closestNodeAtGlobal(gx, gy)
    -- highlight node and show connection options
    if dist < NODE_CONNECT_DIST and node then
      zone:selectNodeView(node.view)
      self:setNode(node)
    else
      zone:selectNodeView(nil)
      self:setNode(nil)
      return
    end
  end

  -- Link of slots of the other type: when key is 'ctrl', 
  -- self.slots_for.ctrl.list is the list of 'param' slots.
  local list = self.slots_for[key]
  list = list and list.list
  if list then
    local best_slot, best_dist
    for _, slot in pairs(list) do
      local d = distanceToSlot(slot, gx, gy)
      if not best_dist or d < best_dist then
        best_slot = slot
        best_dist = d
      end
    end
    if best_dist and best_dist < CONNECT_DIST then
      if best_slot ~= self.target_slot then
        if self.target_slot then
          self.target_slot:select(false)
        end

        best_slot:select(true)
        self.target_slot = best_slot
      end
    elseif self.target_slot then
      self.target_slot:select(false)
      self.target_slot = nil
    end
  end
end

function lib:endDrag(key)
  if self.target_slot then
    -- make link to closest slot
    local s = self.source_slot
    local t = self.target_slot
    local node, param_name, conn, sx, sy, tx, ty
    if key == 'ctrl' then
      node       = t.node
      param_name = t.name
      conn       = s.conn
    else
      node       = s.node
      param_name = s.name
      conn       = t.conn
    end
    self.paths.drag = nil
    self:setPath(self.source_slot, self.target_slot, self.source_slot)
    private.link(self, node, param_name, conn)
  else
    self.source_slot:select(false)
    self.paths[self.source_slot] = nil
    self.source_slot = nil
    self:update()
    self:rebuildPaths()
  end
end

function lib:setPath(slot, x, y, key)
  local x1, y1, x2, y2
  if type(x) == 'table' then
    local target = x
    if slot.conn then
      -- Control to Node param
      x2, y2 = slot:globalPosition()
      y2 = y2 + slot:height() / 2
      x1, y1 = target:globalPosition()
      x1 = x1 + target:width()
      y1 = y1 + target:height() / 2
    else
      -- Node param to Control
      x1, y1 = slot:globalPosition()
      y1 = y1 + slot:height() / 2
      x1 = x1 + target:width()
      x2, y2 = target:globalPosition()
      y2 = y2 + target:height() / 2
    end
  else
    -- drag operation
    if slot.conn then
      -- Control to (x,y)
      x2, y2 = slot:globalPosition()
      y2 = y2 + slot:height() / 2
      x1, y1 = x, y
    else
      -- Node param to (x,y)
      x1, y1 = slot:globalPosition()
      y1 = y1 + slot:height() / 2
      x1 = x1 + slot:width()
      x2, y2 = x, y
    end
  end

  local path = mimas.Path()
  x1, x2 = x1 - self.gx, x2 - self.gx
  y1, y2 = y1 - self.gy, y2 - self.gy

  path:moveTo(x1, y1)
  local vect = math.min(VECH, math.abs(y2 - y1)*0.3 + VECH*0.1);
  vect = math.min(vect, math.abs(x2 - x1) + VECH*0.1);

  path:cubicTo(
    x1 + vect, y1,
    x2 - vect, y2,
    x2, y2
  )

  if key then
    self.paths[key] = {self.drag_pen, path}
  else
    self.paths[slot] = {slot.pen, path}
  end
  self:update()
end

-- Show edit connector dialog
function lib:editDialog(conn, gx, gy)
  local node = conn.node

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
      {'btn', 'Remove'},
      {'btn', 'OK', default = true},
    },
  }

  self:addWidget(dlg)
  dlg:globalMove(gx - 5, gy - 5)
  self.dlg = dlg
  
  function dlg.btn(dlg, btn_name)
    if btn_name == 'OK' then
      local opt = {
        min = tonumber(dlg.form.min) or 0,
        max = tonumber(dlg.form.max) or 1,
      }
      conn.ctrl:change {
        connect = {
          [conn.name] = opt,
        },
      }
    else
      conn.ctrl:change {
        connect = {
          [conn.name] = false,
        },
      }
    end

    self:selectSource(nil)
    dlg:hide()
  end
  dlg:show()
end

--=============================================== Widget callbacks

local noBrush = mimas.EmptyBrush

-- Draw links ?
function lib:paint(p, w, h)
  p:fillRect(0, 0, w, h, self.back_color)
  p:setBrush(noBrush)
  for _, path in pairs(self.paths) do
    p:setPen(path[1])
    p:drawPath(path[2])
  end
end

function lib:click()
  private.cleanup(self)
end
--=============================================== PRIVATE

function private:link(node, param, conn, opts)
  -- absolute url
  local url = node:url() .. '/_/' .. param
  local def = opts or {}
  def.url = url
  if type(node.params[param]) == 'number' then
    def.min = def.min or conn.min or 0
    def.max = def.max or conn.max or 1
  end
  conn.ctrl:change {
    connect = {
      [conn.name] = def,
    },
  }
  if self.target_slot then
    self.target_slot:select(false)
    self.target_slot = nil
  end
  self.source_slot:select(false)
  self.source_slot = nil
  self:rebuildPaths()
end

function private:cleanup()
  self:hide()
  self.zone:selectNodeView(nil)
  self.zone.view.link_editor = nil
  self.ctrl.meta_op = nil
end


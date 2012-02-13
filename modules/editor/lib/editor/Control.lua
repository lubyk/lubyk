--[[------------------------------------------------------

  editor.Control
  --------------

  Base class for all control widgets.

--]]------------------------------------------------------
local lib = lk.SubClass(mimas, 'Widget')
editor.Control = lib

local private = {}
local DRAG_CORNER = 20

--=============================================== METHODS TO REIMPLEMENT

-- Handle mouse event on the control.
function lib:control(x, y)
end

-- Paint control.
function lib:paintControl(p, w, h)
end
--=============================================== PUBLIC
function lib:init(id, view)
  self:initControl(id, view)
  self:setCssClass('control')
end

function lib:changed(key, value)
  self:update()
end

function lib:initControl(id, view)
  self.id   = id
  self.view = view
  -- Sorted list of connectors
  self.connectors = {}
  if view then
    self.zone = view.zone
  end
  self:setHue(math.random())
end

function lib:connector(key)
  return self['conn_'..key]
end

function lib:set(def)
  private.setPosition(self, def)
  private.setConnections(self, def)
end

--=============================================== Class methods
local ctrl = _control
-- Find a control from its name 'lk.Slider' finds the control
-- in _control.lk.Slider.
function lib.getControl(name)
  local ctor = ctrl
  local parts = lk.split(name, '%.')
  for _,part in ipairs(parts) do
    ctor = ctor[part]
    if not ctor then
      print('ERROR: unknown control', name)
      break
    end
  end
  return ctor
end

function lib:delete()
  self:hide()
  for _, conn in ipairs(self.connectors) do
    conn:disconnect()
  end
end
--=============================================== PROTECTED

function lib:setupConnectors(def)
  local connectors = self.connectors
  for key, info in pairs(def) do
    local conn = editor.Connector(self, key, info)
    -- This is to enable faster connector access in controls with
    -- conn_x, conn_y...
    self['conn_'..key] = conn

    -- Keep connectors list sorted
    local i = 1
    while connectors[i] and key >= connectors[i].name do
      i = i + 1
    end
    table.insert(connectors, i, conn)
  end
end

function lib:setHue(h)
  if h then
    self.hue = h
  else
    h = self.hue
  end
  if self.enabled then
    self.fill_color = mimas.Color(h, 0.5, 0.5)
    self.pen = mimas.Pen(4, mimas.Color(h, 0.7, 0.7))
  else
    self.fill_color = mimas.Color(0, 0, 0.5)
    self.pen = mimas.Pen(4, mimas.Color(0, 0, 0.7))
  end
  self.thumb_color = mimas.Color(h, 0, 1, 0.5)
end

function lib:setEnabled(key, enabled)
  -- Should be overwritten when there are more then one
  -- connectors.
  self.enabled = enabled
  self:setHue()
end

--=============================================== Widget callbacks
function lib:resized(w, h)
  self.w = w
  self.h = h
end

local ControlModifier = mimas.ControlModifier
local RightButton     = mimas.RightButton
local MousePress      = mimas.MousePress

function lib:click(x, y, op, btn, mod)
  if self.meta_op then
    local m = self.meta_op
    self.meta_op = nil
    if m.op == 'drag' then
      -- end drag
      self:change {
        x = self:x(),
        y = self:y(),
      }
    elseif m.op == 'resize' then
      -- end drag
      self:change {
        w = self.w,
        h = self.h,
      }
    end
  elseif btn == RightButton or
     mod == mimas.MetaModifier then
    local sx, sy = self:globalPosition()
    private.showContextMenu(self, sx + x, sy + y)
  elseif mod == mimas.ControlModifier then
    local meta = {
      x = x,
      y = y,
    }
    if x > self.w - DRAG_CORNER and y > self.h - DRAG_CORNER then
      -- resize
      meta.op = 'resize'
    else
      -- drag
      meta.op = 'drag'
    end
    self.meta_op = meta
  elseif op == MousePress then
    self.show_thumb = true
    self:control(x, y)
  else
    self.show_thumb = false
    self:update()
  end
end

-- Push GUI change to morph.
function lib:change(def)
  self.view:change {
    [self.id] = def
  }
end

function lib:mouse(x, y)
  if self.meta_op then
    local m = self.meta_op
    if m.op == 'drag' then
      self:move(
        self:x() + x - m.x,
        self:y() + y - m.y
      )
    else
      local w = self.w + x - m.x
      local h = self.h + y - m.y
      m.x = x
      m.y = y
      self:resize(w, h)
    end
  elseif self.enabled then
    self.show_thumb = true
    self:control(x, y)
  end
end

local ghost_color = mimas.Color(0, 0, 0.7, 0.5)
function lib:paintGhost(p, w, h)
  p:fillRect(0, 0, w, h, ghost_color)
end

function lib:paint(p, w, h)
  if self.is_ghost then
    self:paintGhost(p, w, h)
  else
    self:paintControl(p, w, h)
  end
end

--=============================================== PRIVATE
 
function private:setPosition(def)
  if def.x or def.y then
    local x = def.x or widget:x()
    local y = def.y or widget:y()
    self:move(x, y)
  end

  if def.w or def.h then
    local w = def.w or widget:width()
    local h = def.h or widget:height()
    self:resize(w, h)
  end
  if def.hue then
    self:setHue(def.hue)
  end
end

function private:setConnections(def)
  local zone = self.zone
  local connect = def.connect or {}
  for dir, opt in pairs(connect) do
    local connector = self:connector(dir)
    if connector then
      if opt == false then
        -- Remove connection.
        connector:disconnect()
      else
        connector:set(opt, zone)
      end
    else
      printf("Invalid connector name '%s' for widget '%s' of type '%s'", dir, self.id, self.type)
    end
  end
end

function private:showContextMenu(gx, gy)
  if self.is_ghost then
    return false
  end

  local menu = mimas.Menu('')
  if self.menu and not menu:deleted() then
    self.menu:hide()
  end
  self.menu = menu

  for _, conn in ipairs(self.connectors) do
    menu:addAction('link '..conn.name, '', function()
      private.startLink(self, conn)
    end)
  end
  menu:addSeparator()
  menu:addAction('Remove', '', function()
    self:change(false)
  end)

  menu:popup(gx - 5, gy - 5)
end

function private:startLink(conn)
  local ghost = editor.CtrlLinkView(conn, self.zone)

  self.meta_op = {
    op    = 'link',
    conn  = conn,
    ghost = ghost,
  }
end



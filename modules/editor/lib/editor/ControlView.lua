--[[------------------------------------------------------

  editor.ControlView
  ------------------

  Displays a control view with all the populated controls.

--]]------------------------------------------------------
local lib = lk.SubClass(mimas, 'Widget')
editor.ControlView = lib

local private = {}

-- constants
function lib:init(name, def, zone)
  self.cache = {}
  self.name = name
  self.zone = zone
  -- List of widgets by id.
  self.widgets = {}
  -- Disconnected connections.
  self.disconnected = {}
  -- Update view from definition.
  self:update(def)
end

function lib:remove()
  if not self:deleted() then
    self:hide()
    self:__gc()
  end
end

function lib:processConnected(process)
  printf("======== processConnected '%s'", process.name)
  local list = self.disconnected
  -- TODO: reconnect offline widgets
  for i=#list,1,-1 do
    local conn = list[i]
    if conn.process == process then
      printf("processConnected for '%s' '%s'", conn.param_name, process.name)
      -- connect
      private.connectToNode(self, conn)
      table.remove(list, i)
    else
      print(process.name, process, conn.process.name, conn.process)
      printf("not processConnected for '%s' '%s'", conn.param_name, process.name)
    end
  end
end

function lib:update(changes)
  lk.deepMerge(self, 'cache', changes)
  local widgets = self.widgets
  for id, def in pairs(changes) do
    local widget = widgets[id]
    if v == false then
      if widget then
        -- remove widget
        widget:__gc()
        widgets[id] = nil
      end
      return
    elseif widget then
      -- update
    else
      -- create
      local typ = def.type
      local ctor = _ctrl
      local parts = lk.split(typ, '.')
      for _,part in ipairs(parts) do
        ctor = ctor[part]
        if not ctor then
          print('ERROR: unknown control', typ)
          break
        end
      end

      if ctor then
        local widget = ctor(def)
        widget.id = id
        widgets[id] = widget
        self:addWidget(widget)
        private.placeWidget(self, widget, def)
        private.connectWidget(self, widget, def)
        widget:show()
      end
    end
  end
end

function lib:disconnect(conn)
  if not conn.remove then
    table.insert(self.disconnected, conn)
  end
  conn.node = nil
  -- TODO: mark connector as disabled
  -- connection.connector:disable()
end

function private:placeWidget(widget, def)
  if def.x or def.y then
    local x = def.x or widget:x()
    local y = def.y or widget:y()
    widget:move(x + 120, y)
  end

  if def.w or def.h then
    local w = def.w or widget:width()
    local h = def.h or widget:height()
    widget:resize(w, h)
  end
  if def.hue then
    widget:setHue(def.hue)
  end
end

function private:connectWidget(widget, def)
  local connect = def.connect or {}
  for dir, connection in pairs(connect) do
    local connector = widget:connector(dir)
    if connector then
      private.updateConnection(self, connector, connection)
    else
      printf("Invalid connector name '%s' for widget '%s' of type '%s'", dir, widget.id, widget.type)
    end
  end
end

--=============================================== 
------ THESE METHODS SHOULD EXIST IN THE CONTROLS (mimas.Slider, etc)
function private:connectToNode(conn)
  local node = conn.process
  for i, part in ipairs(conn.parts) do
    node = node:findNode(part)
  end
  conn.node = node
  node:connectControl(conn)
end

function private:disconnectFromNode(conn)
  conn.node:disconnectControl(conn)
  conn.node = nil
end

-- Get the connector for the given direction/param.
-- TODO: remove and make 'getConnector' a method in the widgets.
function private.getConnector(widget, dir)
  local typ = widget.type
  if typ == 'mimas.Slider' then
    -- TODO
    -- if dir == 'x' then
    --   widget:setType(mimas.Horizontal)
    -- else
    --   widget:setType(mimas.Vertical)
    -- end
    return widget
  elseif typ == 'mimas.Pad' then
    if dir == 'x' then
      return widget.range_x
    else
      return widget.range_y
    end
  end
end

--=============================================== 
--- self == connector (widget or range)
function private:sliderChanged(value)
  for _, conn in pairs(self.connections) do
    local process = conn.process
    if process.online then
      -- Pointer inside conn.msg to update value
      local param_list = conn.param_list
      param_list[conn.param_name] = value
      process.push:send(lubyk.update_url, conn.msg)
    end
  end
end

--- self == conn
function private:setValue(value)
  if value then
    if type(value) == 'table' then
      -- ERROR_VALUE
      self.connector:error("Remote does not respond to '%s'.", self.param_name)
    else
      self.connector:setValue(value)
    end
  end
end

function private:updateConnection(connector, opt)
  if opt == false then
    -- Delete.
    -- FIXME
    -- connector.connections[target] = nil
    -- private.disconnectFromNode(self, conn)
  else
    local url = opt.url
    if url then
      local process_name = string.match(url, '^([^/])/')
      local process = self.zone:findProcess(process_name)
      connector:connect(process, url)
    end
    -- Set options
    connector:set(opt)
  end
end

function lib:paint(p, w, h)
  p:fillRect(0, 0, w, h, mimas.Color(0, 0, 0.12))
end

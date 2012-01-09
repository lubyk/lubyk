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
  local list = self.disconnected
  -- TODO: reconnect offline widgets
  for i, conn in ipairs(list) do
    if conn.process == process then
      -- connect
      private.connectToNode(self, conn)
      table.remove(list, i)
    end
  end
end

function lib:update(changes)
  printf("UPDATE VIEW: %s", yaml.dump(changes))
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
    elseif widget then
      -- update
      private.updateWidget(self, widget, def)
    else
      -- create
      local typ = def.type
      -- TODO: we could try to retrieve from _G but this would
      -- allow arbitrary code execution on the GUI.... Not sure we want this...
      if typ == 'mimas.Slider' then
        local widget = mimas.Slider(def.orientation or mimas.Vertical)
        widget.id = id
        widgets[id] = widget
        self:addWidget(widget)
        private.setupConnections(self, widget)
        private.placeWidget(self, widget, def)
        private.connectWidget(self, widget, def)
      end
    end
  end
end

function lib:disconnect(connection)
  table.insert(self.disconnected, connection)
  -- TODO: mark connector as disabled
  -- connection.connector:disable()
end

function private:placeWidget(widget, def)
  if def.x or def.y then
    local x = def.x or widget:x()
    local y = def.y or widget:y()
    widget:move(x, y)
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
  for dir, connections in pairs(connect) do
    local connector = private.getConnector(widget, dir)
    for target, opt in pairs(connections) do
      local connected = connector.connections[target]
      if opt == false then
        if connected then
          -- TODO: REMOVE CONNECTION
        end
      elseif connected then
        -- TODO: UPDATE
      else
        if type(opt) ~= 'table' then
          opt = {}
        end
        local parts = lk.split(target, '/')
        -- Remove '' element.
        table.remove(parts, 1)
        local process_name = parts[1]
        local process = self.zone:findProcess(process_name)
        -- Remove process name.
        table.remove(parts, 1)
        local param_name = parts[#parts]
        -- Remove param name.
        table.remove(parts, #parts)
        -- Remove '_' (param indicator).
        table.remove(parts, #parts)
        -- Build msg template.
        local msg = {nodes = {}}
        local path = msg.nodes
        for i, part in ipairs(parts) do
          path[part] = {}
          path = path[part]
        end
        path._ = {}

        -- Create new connection.
        local conn = {
          view       = self,
          process    = process,
          set        = private.setValue,
          -- self during set
          connector  = connector,
          -- params to send to process
          param_name = param_name,
          param_list = path._,
          msg        = msg,
          -- To reconnect
          parts      = parts,
        }
        if opt.min then
          connector:setRange(opt.min, opt.max)
        end
        -- Set connection from widget to process.
        connector.connections[target] = conn
        if process.online then
          -- Set connection from editor.Node to widget.
          private.connectToNode(self, conn)
        else
          self:disconnect(conn)
        end
      end
    end
  end
end

function private:setupConnections(widget)
  widget.connections = {}
  widget.sliderChanged = private.sliderChanged
end

function private:connectToNode(conn)
  local node = conn.process
  for i, part in ipairs(conn.parts) do
    node = node:findNode(part)
  end
  node:connectControl(conn.param_name, conn)
end

--- self == connector (widget or range)
function private:sliderChanged(value)
  for _, conn in pairs(self.connections) do
    local process = conn.process
    if process.online then
      -- Pointer inside conn.msg to update value
      local param_list = conn.param_list
      param_list[conn.param_name] = value
      printf("PUSH CTRL '%s' : %s", process.name, yaml.dump(conn.msg))
      process.push:send(lubyk.update_url, conn.msg)
    else
      printf("NOT ONLINE '%s'", process.name)
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

function private:updateWidget(widget, def)
  -- TODO: change type, pos, etc
  if def.connect then
    for dir, connections in pairs(def.connect) do
      local connector = private.getConnector(widget, dir)
      if connector then
        local current_conn = connector.connections
        -- Update connections.
        for target, opt in pairs(connections) do
          local conn = current_conn[target]
          if opt == false then
            -- Disconnect.
            current_conn[target] = nil
          else
            if conn then
              -- Should we clear old ?
            end
            -- Connect.
            private.connectWidget(self, widget, def)
          end
        end
      end
    end
  end
end

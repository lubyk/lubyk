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
  self.name = name
  self.zone = zone
  self.widgets = {}
  -- Update view from definition
  self:update(def)
end

function lib:remove()
  if not self:deleted() then
    self:hide()
    self:__gc()
  end
end

function lib:update(changes)
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
        local name, url = string.match(target, '^/([^/]+)/(.*)$')
        -- Create new connection
        local process = self.zone:findProcess(name)
        local list = process.controls[url]
        if not list then
          list = {}
          process.controls[url] = list
        end
        local conn = {
          process = process,
          url     = url, 
          set     = private.setValue,
          connector  = connector,
        }
        if opt.min then
          connector:setRange(opt.min, opt.max)
        end
        connector.connections[target] = conn
        table.insert(list, conn)
        -- Query for current value (we will get a notification back)
        -- TODO: remove once we have settings in process notifications and
        -- dump !
        if process.online then
          private.sliderChanged(connector, nil)
        else
          -- we will query when it comes online
        end
      end
    end
  end
end

function private:setupConnections(widget)
  widget.connections = {}
  widget.sliderChanged = private.sliderChanged
end

--- self == widget
function private:sliderChanged(value)
  for _, conn in pairs(self.connections) do
    local process = conn.process
    if process.online then
      process.push:send(lubyk.control_url, conn.url, value)
    end
  end
end

--- self == conn
function private:setValue(value)
  if value then
    self.connector:setValue(value)
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

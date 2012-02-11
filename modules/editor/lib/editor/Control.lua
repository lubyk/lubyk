--[[------------------------------------------------------

  editor.Control
  --------------

  Base class for all control widgets.

--]]------------------------------------------------------
local lib = lk.SubClass(mimas, 'Widget')
editor.Control = lib

local private = {}

--=============================================== PUBLIC
function lib:init(name)
  self.name = name
  self.connectors = {}
end

function lib:connector(key)
  return self['conn_'..key]
end

function lib:set(def, zone)
  private.setPosition(self, def)
  process.setConnections(self, def, zone)
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

--=============================================== PROTECTED
function lib:setupConnectors(def)
  local conn = self
  for key, info in pairs(def) do
    conn['conn_'..key] = editor.Connector(key, self, info)
  end
end

--=============================================== PRIVATE
 
function private:setPosition(def)
  if def.x or def.y then
    local x = def.x or widget:x()
    local y = def.y or widget:y()
    self:move(x + 120, y)
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

function private:setConnections(def, zone)
  local connect = def.connect or {}
  for dir, opt in pairs(connect) do
    local connector = self:connector(dir)
    if connector then
      if opt == false then
        -- Remove connection.
        connector:disconnect()
      else
        local url = opt.url
        if url then
          local process_name = string.match(url, '^([^/])/')
          local process = zone:findProcess(process_name)
          connector:connect(process, url)
        end
        -- Set options
        connector:set(opt)
      end
    else
      printf("Invalid connector name '%s' for widget '%s' of type '%s'", dir, self.id, self.type)
    end
  end
end



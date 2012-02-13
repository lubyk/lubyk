--[[------------------------------------------------------

  editor.Connector
  ----------------

  Element to receive control notifications and push
  changes to processing tree.

--]]------------------------------------------------------
local lib = {type = 'editor.Connector'}
lib.__index      = lib
editor.Connector = lib
local private    = {}

--=============================================== PUBLIC
setmetatable(lib, {
  __call = function(lib, ctrl, name, info)
    local self = {
      ctrl  = ctrl,
      name  = name,
      info  = info,
      -- Value as notified by remote end.
      remote_value = 0,
      -- Value set by GUI (0-1 scale).
      value = 0,
    }
    return setmetatable(self, lib)
  end,
})

local UPDATE_URL = lubyk.update_url

function lib:set(def, zone)
  local url = def.url
  local connecting
  if url and url ~= self.url then
    connecting = true
    local process_name = string.match(url, '^/([^/]+)/')
    self.process = zone:findProcess(process_name)
    private.connect(self, self.process, url)
  end

  -- Set options
  local min = tonumber(def.min) or 0
  self.min = min
  local max = tonumber(def.max) or 1
  self.max = max
  local range = max - min
  if min == 0 and max == 1 or range == 0 then
    range = nil
  end
  self.range = range

  -- upvalues
  local name       = self.name
  local param_name = self.param_name
  local msg        = self.msg
  local process    = self.process
  local setter     = self.setter
  local ctrl       = self.ctrl
  local changed    = self.ctrl.changed

  if not range then
    function self.change(value)
      self.value = value
      if process.online then
        setter[param_name] = value
        process.push:send(UPDATE_URL, msg)
      end
    end
    -- ! No 'self' here.
    function self.changed(value)
      self.remote_value = value
      changed(ctrl, name, value)
    end
  else
    function self.change(value)
      self.value = value
      if process.online then
        setter[param_name] = min + value * range
        process.push:send(UPDATE_URL, msg)
      end
    end
    -- ! No 'self' here.
    function self.changed(value)
      self.remote_value = value
      changed(ctrl, name, (value-min) / range)
    end
  end

  if connecting then
    -- We do this last, when change/changed callbacks are set.
    self.node:connectControl(self)
  end
end

function lib:setEnabled(enabled)
  self.ctrl:setEnabled(self.name, enabled)
end

function lib:disconnect()
  self.change = lib.change
  if self.node then
    self.node:disconnectControl(self)
  end
  self.node   = nil
end

function lib:change()
  -- Not connected yet. The control should disable controls instead
  -- of having this called...
  -- noop
end

-- Text representation of value
function lib:printValue()
  local v = self.remote_value
  if v > 10 then
    return string.format('%i', v)
  else
    return string.format('%.2f', v)
  end
end

--=============================================== PRIVATE
function private:connect(process, url)
  self.url = url

  -- Takes a target link like '/a/metro/_/tempo'
  -- /[process]/[node](/[sub-node])/_/[param name]
  local parts = lk.split(url, '/')
  -- Remove '' element.
  table.remove(parts, 1)
  -- Remove process name.
  table.remove(parts, 1)
  local param_name = parts[#parts]
  self.param_name = param_name
  -- Remove param name.
  table.remove(parts, #parts)
  -- Remove '_' (param indicator).
  table.remove(parts, #parts)
  -- Build msg template.
  local msg = {nodes = {}}
  local path = msg.nodes
  local node = process
  for i, part in ipairs(parts) do
    path[part] = {}
    path = path[part]
    -- This cannot fail because 'findNode' creates pending nodes
    -- when needed.
    node = node:findNode(part)
  end
  path._ = {}

  self.param_name = param_name
  self.setter = path._
  self.msg    = msg
  self.node   = node
  self:setEnabled(node.online)
end


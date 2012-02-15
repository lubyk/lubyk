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
      -- Scaled remote value.
      remote_value = 0,
      -- Unscaled remote value.
      raw_remote_value = 0,
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
  local min = self.min
  local max = self.max
  local range = self.range
  if def.min or def.max then
    min = tonumber(def.min) or 0
    max = tonumber(def.max) or 1
    range = max - min
    self.min = min
    self.max = max
    self.range = range

    self.inverted = min > max
  end

  -- upvalues
  local inverted   = self.inverted
  local name       = self.name
  local param_name = self.param_name
  local msg        = self.msg
  local process    = self.process
  local setter     = self.setter
  local ctrl       = self.ctrl
  local changed    = self.ctrl.changed
  -- last notification: We do not send changes before receiving
  -- notification of last change or waiting 100 ms.
  -- WE HAVE COMMENTED CODE WITH AN EXPERIMENTAL SYSTEM TO AVOID
  -- CONTROL OVERFLOW AT END OF THIS FILE.

  if not range then
    --=============================================== Raw value
    -- not a number value
    function self.change(value)
      local now = worker:now()
      self.value = value
      if process.online then
        setter[param_name] = value
        process.push:send(UPDATE_URL, msg)
      end
    end
    -- ! No 'self' here.
    function self.changed(value)
      self.remote_value = value
      self.raw_remote_value = value
      changed(ctrl, name, value)
    end


  elseif self.min == 0 and self.max == 1 then
    --=============================================== Number, no scaling
    function self.change(value)
      if value < min then
        value = min
      elseif value > max then
        value = max
      end
      self.value = value
      if process.online then
        setter[param_name] = value
        process.push:send(UPDATE_URL, msg)
      end
    end
    -- ! No 'self' here.
    function self.changed(value)
      self.raw_remote_value = value
      self.remote_value = value
      changed(ctrl, name, value)
    end


  else
    --=============================================== Number, with scaling
    function self.change(value)
      if value < 0 then
        value = 0
      elseif value > 1 then
        value = 1
      end
      self.value = value
      if process.online then
        setter[param_name] = min + value * range
        process.push:send(UPDATE_URL, msg)
      end
    end
    -- ! No 'self' here.
    function self.changed(value)
      -- value before scaling
      self.raw_remote_value = value
      value = (value - min) / range
      self.remote_value = value
      changed(ctrl, name, value)
    end
  end

  if connecting then
    -- We do this last, when change/changed callbacks are set.
    self.node:connectConnector(self)
  end
end

function lib:setEnabled(enabled)
  self.enabled = enabled
  if not self.ctrl:deleted() then
    self.ctrl:setEnabled(self.name, enabled)
  end
end

function lib:disconnect()
  self.url = nil
  self.change = lib.change
  self:setEnabled(false)
  if self.node then
    self.node:disconnectConnector(self)
  end
end

function lib:change()
  -- Not connected yet. The control should disable controls instead
  -- of having this called...
  -- noop
end

-- Text representation of value
function lib:printValue()
  local v = self.raw_remote_value
  if v > 10 then
    return string.format('%i', v)
  else
    return string.format('%.2f', v)
  end
end

--=============================================== PRIVATE
function private:connect(process, url)
  if self.url then
    -- disconnect
    if self.node then
      self.node:disconnectConnector(self)
    end
  end

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

--[[
--EXPERIMENTAL CONTROL BUFFER OVERFLOW PROTECTION.
  local last_not   = worker:now()
  local last_send  = 0
  local WAIT_RETRY = 100 -- ms
  local to_send

  if not range then
    --=============================================== Raw value
    -- not a number value
    function self.change(value)
      local now = worker:now()
      self.value = value
      if process.online then
        if last_send < last_not or
           now > last_send + WAIT_RETRY then
          last_send = now
          setter[param_name] = value
          process.push:send(UPDATE_URL, msg)
        else
          to_send = value
        end
      end
    end
    -- ! No 'self' here.
    function self.changed(value)
      last_not = worker:now()
      self.remote_value = value
      self.raw_remote_value = value
      changed(ctrl, name, value)
      local ts = to_send
      if ts then
        to_send = nil
        self.change(ts)
      end
    end


  elseif self.min == 0 and self.max == 1 then
    --=============================================== Number, no scaling
    function self.change(value)
      local now = worker:now()
      if value < min then
        value = min
      elseif value > max then
        value = max
      end
      self.value = value
      if process.online then
        if last_send < last_not or
           now > last_send + WAIT_RETRY then
          last_send = now
          setter[param_name] = value
          process.push:send(UPDATE_URL, msg)
        else
          to_send = value
        end
      end
    end
    -- ! No 'self' here.
    function self.changed(value)
      last_not = worker:now()
      self.raw_remote_value = value
      self.remote_value = value
      changed(ctrl, name, value)
      local ts = to_send
      if ts then
        to_send = nil
        self.change(ts)
      end
    end


  else
    --=============================================== Number, with scaling
    function self.change(value)
      local now = worker:now()
      if value < 0 then
        value = 0
      elseif value > 1 then
        value = 1
      end
      self.value = value
      if process.online then
        if last_send < last_not or
           now > last_send + WAIT_RETRY then
          last_send = now
          setter[param_name] = min + value * range
          process.push:send(UPDATE_URL, msg)
        else
          to_send = value
        end
      end
    end
    -- ! No 'self' here.
    function self.changed(value)
      last_not = worker:now()
      -- value before scaling
      self.raw_remote_value = value
      value = (value - min) / range
      self.remote_value = value
      changed(ctrl, name, value)
      local ts = to_send
      if ts then
        to_send = nil
        self.change(ts)
      end
    end
  end
--]]

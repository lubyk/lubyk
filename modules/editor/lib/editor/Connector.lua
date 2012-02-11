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
      ctrl = ctrl,
      name = name,
      info = info,
    }
    local changed = ctrl.changed
    -- ! No 'self' here.
    self.changed = function(...)
      changed(ctrl, name, ...)
    end
    return setmetatable(self, lib)
  end,
})

local UPDATE_URL = lubyk.update_url

function lib:connect(process, url)
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

  local setter = path._
  function self.change(value)
    if process.online then
      setter[param_name] = value
      process.push:send(UPDATE_URL, msg)
    end
  end

  self.node = node
  node:connectControl(self)
  self:setEnabled(node.online)
end

function lib:setEnabled(enabled)
  -- TODO: mark view as on/off
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

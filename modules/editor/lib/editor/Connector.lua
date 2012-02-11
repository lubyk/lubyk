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
  -- Takes a target link like '/a/metro/_/tempo'
  -- /[process]/[node](/[sub-node])/_/[param name]
  local parts = lk.split(url, '/')
  -- Remove '' element.
  table.remove(parts, 1)
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

  local setter = path._
  function self.change(value)
    if process.online then
      setter[param_name] = value
      process.push:send(UPDATE_URL, msg)
    end
  end
end

function lib:change()
  -- Not connected yet. The control should disable controls instead
  -- of having this called...
  -- noop
end

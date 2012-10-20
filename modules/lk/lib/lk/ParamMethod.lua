--[[------------------------------------------------------

  lk.ParamMethod
  --------------

  Creates the param set and notify method 'param' used in
  nodes.

--]]------------------------------------------------------
local lib = {type='lk.ParamMethod'}
lk.ParamMethod = lib

setmetatable(lib, {
  -- new method
 __call = function(lib, node)
  -- Create param method/accessor for a given node
  local self = {
    node   = node,
    inlets = node.inlets,
    env    = node.env,
  }
  -- Takes a target link like '/a/metro/_/tempo'
  -- /[process]/[node](/[sub-node])/_/[param name]
  local target = node:url()
  local parts = lk.split(target, '/')
  -- Remove '' element and process name.
  table.remove(parts, 1)
  table.remove(parts, 1)
  -- [node, sub-node, ...]
  -- Build msg template.
  local msg = {nodes = {}}
  local path = msg.nodes
  for i, part in ipairs(parts) do
    -- mst.nodes[part] = {}
    path[part] = {}
    -- move down
    path = path[part]
  end
  self.msg = msg
  self.param_list = path
  setmetatable(self, lib)
  return self
end})

function lib.__newindex(self, k, value)
  local env = self.env
  if type(value) == 'function' then
    -- Create an accessor for parameter 'k'
    -- or the special 'changed' callback.
    self.node.accessors[k] = value
  elseif type(value) == 'table' then
    -- Change parameter 'k' and notify.
    -- param.pos= {x = 1, y = 2}
    -- This is used from inside lk.Node.
    local old = env[k]
    if not old then
      -- This is an error
      error("Setting invalid parameter '%s'", k)
    end
    local changed = false
    local notify = {}
    for sk, sv in pairs(value) do
      if old[sk] ~= sv then
        changed = true
        old[sk] = sv
        notify[sk] = sv
      end
      if changed then
        -- Notify
        self.param_list._ = {
          [k] = notify,
        }
        self.node.process:notify(self.msg) 
      end
    end
  else
    -- Change parameter 'k' and notify.
    -- param.foo = 45.5
    -- This is used from inside lk.Node.
    local old = env[k]
    if old ~= value then
      env[k] = value
      -- Notify
      self.param_list._ = {
        [k] = env[k],
      }
      self.node.process:notify(self.msg) 
    end
  end
end


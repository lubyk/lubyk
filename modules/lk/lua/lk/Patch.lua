--[[------------------------------------------------------

  lk.Patch
  --------

  A patch provides introspection and edit/restore features
  for nodes.

--]]------------------------------------------------------
local lib   = {type='lk.Patch'}
lib.__index = lib
lk.Patch    = lib

setmetatable(lib, {
  -- new method
 __call = function(table, filepath)
  local instance = {nodes = {}, filepath=filepath, pending_nodes={}}
  setmetatable(instance, lib)
  if lk.exist(filepath) then
    -- only load if file exists
    instance:load(filepath)
  end
  return instance
end})

function lib:set(definitions)
  lk.with_filepath(self.filepath, function()
    -- move to patch file directory
    for name, def in pairs(definitions) do
      -- parsing each node
      local node = self.nodes[name]
      if not node then
        node = lk.Node(self, name)
      end
      node:set(def, self)
    end
  end)
end

function lib:load(path)
  local nodes = yaml.loadpath(path)
  self.filepath = path
  self:set(nodes)
end

function lib:inlet_pending(node_name, inlet_name)
  local node = self.nodes[node_name]
  local inlet = nil
  if node then
    -- inlet not created yet
    inlet = lk.Inlet(inlet_name)
    node.inlets_pending[node_name] = inlet
  else
    -- node not created yet
    local pending_node = self.pending_nodes[node_name]
    if not pending_node then
      pending_node = {}
      self.pending_nodes[node_name] = pending_node
    end
    inlet = pending_node[inlet_name]
    if not inlet then
      inlet = lk.Inlet(inlet_name)
      pending_node[inlet_name] = inlet
    end
  end
  return inlet
end

function lib:get(url, mt)
  local parts = lk.split(url, '/')
  local c = self
  local c_type = 'patch'
  for i, name in ipairs(parts) do
    if i == 1 and name == '' then
      -- TODO: find root patch
    elseif c_type == 'patch' then
      c, c_type = c.nodes[name], 'node'
    elseif c_type == 'node' then
      if name == 'in' then
        c, c_type = c.inlets, 'slist'
      elseif name == 'out' then
        c, c_type = c.outlets, 'slist'
      else
        c, c_type = c[name], '???'
      end
    elseif c_type == 'slist' then
      -- slot list
      c, c_type = c[name], 'slot'
    else
      -- error
      c = nil
    end
    if c == nil then
      return nil, string.format("Cannot find '%s' in '%s'.", name, url)
    end
  end
  local c_mt = getmetatable(c)
  if mt and mt ~= c_mt then
    local msg = ''
    if mt.type and c_mt.type then
      msg = string.format('expected %s, found %s', mt.type, c_mt.type)
    elseif c.__tostring then
      msg = string.format('found %s', c:to__string())
    elseif not c then
      msg = 'found nil'
    else
      msg = 'found ?'
    end
    return false, string.format("Invalid object at '%s' (%s).", url, msg)
  else
    return c
  end
end

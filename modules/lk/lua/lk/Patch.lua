--[[------------------------------------------------------

  lk.Patch
  --------

  A Patch provides introspection and edit/restore features
  for a set of nodes.

--]]------------------------------------------------------
local lib   = {type='lk.Patch'}
lib.__index = lib
lk.Patch    = lib

-- PRIVATE

local function loadFromFilepath(self, filepath)
  if lk.exist(filepath) then
    -- only load if file exists
    local data = yaml.loadpath(filepath)
    -- clear before loading (yaml contains a full definition)
    self.nodes = {}
    self:set(data)
  else
    --print(string.format("'%s' not found.", filepath))
  end
end

local function loadFromYaml(self, yaml_code)
  local data = yaml.load(yaml_code)
  -- clear before loading (yaml contains a full definition)
  self.nodes = {}
  self:set(data)
end


-- PUBLIC

setmetatable(lib, {
  -- new method
 __call = function(lib, filepath_or_code, is_process)
  local instance  = {
    nodes         = {},
    pending_nodes = {},
  }
  setmetatable(instance, lib)
  if not filepath_or_code then
    return instance
  end

  if string.match(filepath_or_code, '\n') then
    -- set filepath to the script containing calling lk.Patch()
    -- this is where the content will be saved
    if is_process then
      instance.filepath   = lk.file(-2)
      instance.is_process = true
    else
      instance.filepath = lk.file(-1)
    end

    -- store full script in filepath
    instance.inline   = true
    loadFromYaml(instance, filepath_or_code)
  else
    instance.filepath = filepath_or_code
    -- store only yaml in filepath
    instance.inline   = false
    loadFromFilepath(instance, filepath_or_code)
  end
  return instance
end})

local function setNodes(self, nodes_definition)
  local nodes = self.nodes
  lk.with_filepath(self.filepath, function()
    -- move to patch file directory
    for name, def in pairs(nodes_definition) do
      -- parsing each node
      local node = nodes[name]
      if not node then
        node = lk.Node(self, name)
      end
      node:set(def, self)
    end
  end)
end

function lib:set(definitions)
  for k, v in pairs(definitions) do
    if k == 'nodes' then
      setNodes(self, v)
    elseif k == 'x' or k == 'y' or k == 'w' or k == 'h' then
      self[k] = v
    end
  end
end

-- Create a pending inlet.
function lib:pendingInlet(inlet_url)
  local parts = lk.split(inlet_url, '/')
  local node_name, inlet_name
  if #parts == 3 and parts[2] == 'in' then
    node_name, inlet_name = parts[1], parts[3]
  else
    -- FIXME: store absolute path for 'inlet_url' in process pending list
    -- and resolve this list on node creation
    return nil, string.format("Invalid link url '%s' (target not found and cannot create temporary).", inlet_url)
  end

  local node = self.nodes[node_name]
  local inlet = nil
  if node then
    -- inlet not created yet
    inlet = lk.Inlet(inlet_name)
    node.pending_inlets[inlet_name] = inlet
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

-- Find an element from an url (also used by editor.Process)
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
      return nil, string.format("Cannot find '%s' while resolving '%s'.", name, url)
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

--- Serialize current patch as a lua table. If a
-- data table is provided, only dump parts that contain
-- keys in the table.
function lib:dump(data)
  local res = {
    nodes = {},
    x = self.x,
    y = self.y,
    w = self.w,
    h = self.h,
  }
  local nodes = res.nodes
  for k, node in pairs(self.nodes) do
    nodes[k] = node:dump()
  end
  return res
end

--- Serialize part of the current patch as a lua table by only returning
-- data for parts that contain keys in the given table.
function lib:partialDump(data)
  local res = {}
  for k, v in pairs(data) do
    if k == 'nodes' then
      res.nodes = {}
      local nodes = res.nodes
      for k,node_data in pairs(v) do
        local node = self.nodes[k]
        if node then
          nodes[k] = node:partialDump(node_data)
        else
          nodes[k] = false
        end
      end
    elseif k == 'x' or k == 'y' or k == 'w' or k == 'h' then
      res[k] = self[k]
    end
  end
  return res
end

--- Process related stuff ---

--- Answering requests to Process.
function lib:callback(url, data)
  if url == lubyk.dump_url then
    -- sync call, return content
    return self:dump()
  elseif url == lubyk.update_url then
    -- async call, no return value
    self:set(data)
    self:notify(self:partialDump(data))
  end
end

--- Inform listening editors that something changed.
function lib:notify(changes)
  local service = self.service
  if service then
    service:notify(changes)
  end
end
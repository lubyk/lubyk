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
local ALLOWED_KEYS = {
  nodes = true,
  x = true,
  y = true,
  w = true,
  h = true,
  hue = true,
}

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

local function setFilePath(self, filepath)
  self.filepath = filepath
  -- [work_dir]/PatchName/patch_file.[lua|yml]
  local patch_base = lk.directory(filepath)
  self.name = string.match(patch_base, '([^%./]+)$')
  self.work_dir = lk.directory(patch_base)
end

-- PUBLIC

setmetatable(lib, {
  -- new method
 __call = function(lib, filepath_or_code, is_process)
  local self  = {
    nodes         = {},
    pending_nodes = {},
    work_dir      = lfs.currentdir(),
  }
  setmetatable(self, lib)
  if not filepath_or_code then
    return self
  end

  if string.match(filepath_or_code, '\n') then
    -- set filepath to the script containing calling lk.Patch()
    -- this is where the content will be saved
    if is_process then
      setFilePath(self, lk.file(-2))
      self.is_process = true
    else
      setFilePath(self, lk.file(-1))
    end

    -- store full script in filepath
    self.inline   = true
    loadFromYaml(self, filepath_or_code)
  else
    setFilePath(self, filepath_or_code)
    -- store only yaml in filepath
    self.inline   = false
    loadFromFilepath(self, filepath_or_code)
  end
  return self
end})

local function setNodes(self, nodes_definition)
  local nodes = self.nodes
  lk.withDirectory(self.work_dir, function()
    -- move to patch file directory
    for name, def in pairs(nodes_definition) do
      -- parsing each node
      local node = nodes[name]
      if not node then
        node = lk.Node(self, name)
      end
      node:set(def)
    end
  end)
end

function lib:set(definitions)
  for k, v in pairs(definitions) do
    if k == 'nodes' then
      setNodes(self, v)
    elseif ALLOWED_KEYS[k] then
      self[k] = v
    end
  end
end

--- Create a pending inlet from an url relative to this process (nearly the same
-- as editor.Process.pendingInlet).
function lib:pendingInlet(inlet_url)
  -- inlet_url example:
  --   node/in/slot
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

-- Find an element from an url. An url
-- is something like
--     /process name/[parent node name]*/node name/in/inlet name
-- or  /process name/[parent node name]*/node name
-- or  [parent node name]*/node name
--
-- ALSO USED BY editor.Process
function lib:get(url, mt)
  local parts = lk.split(url, '/')
  -- current object
  local c

  -- type of next current object
  local c_next
  if string.match(url, '^/') then
    c_next = 'process'
    table.remove(parts, 1)
  else
    c = self
    c_next = 'node'
  end

  for i, name in ipairs(parts) do
    if c_next == 'process' then
      c, c_next = self:findProcess(name), 'node'
    elseif c_next == 'node' then
      -- Find node
      if name == 'in' then
        c, c_next = c.inlets, 'slot'
      elseif name == 'out' then
        c, c_next = c.outlets, 'slot'
      else
        -- sub-node 'a/b' lives in nodes.a.nodes.b
        c = c.nodes[name]
      end
    elseif c_next == 'slot' then
      -- slot list
      c, c_type = c[name], nil
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

function lib:url()
  return '/' .. self.name
end

--- Serialize current patch as a lua table. If a
-- data table is provided, only dump parts that contain
-- keys in the table.
function lib:dump(data)
  local res = {nodes = {}}
  for k, _ in pairs(ALLOWED_KEYS) do
    if k == 'nodes' then
      local nodes = {}
      res.nodes = nodes
      for k, node in pairs(self.nodes) do
        nodes[k] = node:dump()
      end
    else
      local v = self[k]
      if v then
        res[k] = v
      end
    end
  end
  return res
end

--- Serialize part of the current patch as a lua table by only returning
-- data for parts that contain keys in the given table.
function lib:partialDump(data)
  local res = {}
  for k, v in pairs(data) do
    if ALLOWED_KEYS[k] then
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
      else
        res[k] = self[k]
      end
    end
  end
  return res
end

--- Find source code for the node at the given url (patch_name/[parent_node/]node_name).
--[[

  Path organization (local filesystem or remote on Mnémosyne server):

    ... <== base project directory. This is the current dir for the process
    .../A              <== Everything for process with role "A"
    .../A/_patch.yml   <== Patch for process with role "A"
    .../B              <== Everything for process with role "B"
    .../B/_patch.yml   <== Patch for process with role "B"
    .../B/foo.lua or
    ...[node:url()].lua <== Code for node 'foo' in process 'B'
    .../lib             <== searched for required code
--]]
function lib:findCode(url)
  local filepath = '.' .. url .. '.lua'
  -- TODO: adapt for Mnémosyne
  return lk.readall(filepath)
end

function lib:findClass(class_name)
  -- TODO: adapt for Mnémosyne
  return lk.findcode(class_name)
end

--=====================================================================
--================= Process related methods ===========================

--- Answering requests to Process.
function lib:callback(url, data)
  if url == lubyk.dump_url then
    -- sync call, return content
    return self:dump()
  elseif url == lubyk.update_url then
    -- async call, no return value
    --print(yaml.dump(data))
    app:post(function()
      -- We eval code in app (GUI) thread so that
      -- object (Widgets) creation is done in this thread.
      self:set(data)
      self:notify(self:partialDump(data))
    end)
  end
end

--- Inform listening editors that something changed.
function lib:notify(changes)
  local service = self.service
  if service then
    service:notify(changes)
  end
end

--- In order to resolve cross-process links, return a process by a given name.
-- ALSO USED BY editor.Process
function lib:findProcess(name)
  if name == self.name then
    return self
  else
    -- FIXME: found process on network...
    return nil
  end
end

--- Return the patches url. This is the same as the patch's name for non-nested
-- nodes.
function lib:url()
  return '/' .. self.name
end
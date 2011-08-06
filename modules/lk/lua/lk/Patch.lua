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
  if patch_base == '.' then
    patch_base = lfs.currentdir()
  end
  self.name = string.match(patch_base, '([^%./]+)$')
  self.work_dir = lk.directory(patch_base)
end

-- PUBLIC

setmetatable(lib, {
  -- new method
 __call = function(lib, filepath_or_code, zone)
  local self  = {
    zone              = zone,
    nodes             = {},
    pending_nodes     = {},
    pending_processes = {},
    found_processes   = {},
    work_dir          = lfs.currentdir(),
  }
  setmetatable(self, lib)
  if not filepath_or_code then
    return self
  end
  if zone then
    -- Create processes watch before loading code (to resolve remote
    -- processes).
    --
    --- Watch for other processes on the network and create
    -- lk.RemoteProcess proxies when needed.
    self.process_watch = lk.ProcessWatch(self)
  end

  if string.match(filepath_or_code, '\n') then
    -- set filepath to the script containing calling lk.Patch()
    -- this is where the content will be saved
    if zone then
      -- this is a process
      -- When we load from morph, we must set file loader to be the
      -- remote morph server. (find code)
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
        nodes[name] = node
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
    return nil, string.format("Invalid pendingInlet url '%s'.")
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
  -- not greedy regexp
  local process_name, path = string.match(url, '^/([^/]*)/(.*)$')

  local process
  if process_name then
    process = self:findProcess(process_name)
  else
    process = self
    path    = url
  end

  local res = process:findByPath(path)
  local res_mt = getmetatable(res)
  if res and mt and mt ~= res_mt then
    local msg = ''
    if mt.type and res_mt.type then
      msg = string.format('expected %s, found %s', mt.type, res_mt.type)
    elseif res.__tostring then
      msg = string.format('found %s', res:to__string())
    elseif not res then
      msg = 'found nil'
    else
      msg = 'found ?'
    end
    return false, string.format("Invalid object at '%s' (%s).", url, msg)
  else
    return res
  end
end

-- FIXME: cache relative url !
--
-- ALSO USED BY editor.Process
function lib:findByPath(path)
  local parts = lk.split(path, '/')
  -- current object
  local c = self
  local c_next = 'node'

  for i, name in ipairs(parts) do
    if c_next == 'node' then
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
      return nil, string.format("Cannot find '%s' while resolving '%s'.", name, path)
    end
  end
  return c
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
  -- TODO: adapt for Morph
  return lk.findCode(class_name)
end

--===========================================================
--================= Process related methods ===========================

--- Answering requests to Process.
function lib:callback(url, ...)
  if url == lubyk.dump_url then
    -- sync call, return content
    return self:dump()
  elseif url == lubyk.update_url then
    local data = ...
    -- async call, no return value
    --print(yaml.dump(data))
    app:post(function()
      -- We eval code in app (GUI) thread so that
      -- object (Widgets) creation is done in this thread.
      self:set(data)
      self:notify(self:partialDump(data))
    end)
  else
    -- Inter process communication
    local inlet = self:get(url)
    if inlet then
      inlet.receive(...)
    else
      print('Received calls on missing inlet', url)
    end
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
function lib:findProcess(process_name)
  if process_name == self.name then
    return self
  elseif self.process_watch then
    -- RemoteProcess
    local process = self.found_processes[process_name] or
                    self.pending_processes[process_name]
    if not process then
      process = lk.RemoteProcess(process_name)
      self.pending_processes[process_name] = process
    end
    return process
  else
    -- TODO: better error handling
    print(string.format("Cannot find process '%s' (no lk.ProcessWatch).", process_name))
    return nil
  end
end

--- Return the patches url. This is the same as the patch's name for non-nested
-- nodes.
function lib:url()
  if not self.name then
    print(debug.traceback())
  end
  return '/' .. self.name
end

--=============================================== ProcessWatch delegate

-- Callback on found process in ProcessWatch. Not used for the moment.
function lib:addService(remote_service)
  if remote_service.zone ~= self.zone then
    -- not in our zone: ignore
    return
  end

  local service_name = remote_service.name
  if service_name then
    -- this is a process
    local process = self.pending_processes[service_name]

    if process then
      -- connect
      self.pending_processes[service_name] = nil
    else
      -- create new
      process = lk.RemoteProcess(service_name, remote_service)
    end
    self.found_processes[service_name] = process
    process:connect(remote_service)
  else
    -- this is the morph server
  end
end

-- Callback on removed process in ProcessWatch. Not used for the moment.
function lib:removeService(remote_service)
  if remote_service.zone ~= self.zone then
    -- not in our zone: ignore
    return
  end

  local service_name = remote_service.name
  if service_name then
    -- process
    -- transform to pending process
    local process = self.found_processes[service_name]
    if process then
      process:disconnect()
      self.found_processes[service_name] = nil
      self.pending_processes[service_name] = process
    end
  else
    -- morph going offline
  end
end
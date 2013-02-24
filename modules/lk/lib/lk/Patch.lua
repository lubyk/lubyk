--[[------------------------------------------------------

  lk.Patch
  --------

  A Patch provides introspection and edit/restore features
  for a set of nodes.

--]]------------------------------------------------------
local lib     = {type='lk.Patch'}
lib.__index   = lib
lk.Patch      = lib
local private = {}

Lubyk.mimas_quit_on_close = false

-- PRIVATE
local ALLOWED_KEYS = {
  nodes  = true,
  x      = true,
  y      = true,
  w      = true,
  h      = true,
  hue    = true,
  name   = true,
}

-- PUBLIC

setmetatable(lib, {
  -- new method
 __call = function(lib, name)
  local self  = {
    name          = name,
    nodes         = {},
    pending_nodes = {},
    on_notify     = setmetatable({}, {__mode = 'k'}),
  }
  sched.patch = name
  setmetatable(self, lib)

  return self
end})


function lib:start()
  -- When we load from morph, we must set file loader to be the
  -- remote morph server. (find code)
  self.is_process = true

  -- Create processes watch before loading code (to resolve remote
  -- processes).
  --
  --- Watch for other processes on the network and create
  -- lk.RemoteProcess proxies when needed.
  self.process_watch = lk.ProcessWatch():addDelegate(self)
end

function lib:set(definitions)
  --print(string.format("================================= lk.Patch %s\n%s", self.name, yaml.dump(definitions)))
  --print("================================= lk.Patch ]")
  for k, v in pairs(definitions) do
    if k == 'nodes' then
      private.setNodes(self, v)
    elseif ALLOWED_KEYS[k] then
      self[k] = v
    end
  end
  if self.need_cleanup then
    self.need_cleanup = nil
    collectgarbage('collect')
    collectgarbage('collect')
  end
end

--- Create a pending inlet from an url relative to this process (nearly the same
-- as editor.Process.pendingInlet).
function lib:pendingInlet(inlet_abs_url)
  local inlet_url = lk.absToRel(inlet_abs_url, self:url())
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
    inlet = lk.Inlet(inlet_abs_url, inlet_name)
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
      -- We pass absolute url so that the inlet can answer 'url()' requests.
      inlet = lk.Inlet(inlet_abs_url, inlet_name)
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
-- TODO: cache result by full url ?
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
  local nodes = self.nodes
  for k, v in pairs(data) do
    if k == 'nodes' then
      res.nodes = {}
      local res_nodes = res.nodes
      for k, node_data in pairs(v) do
        local node = nodes[k]
        if node then
          res_nodes[k] = node:partialDump(node_data)
        else
          res_nodes[k] = false
        end
      end
    elseif ALLOWED_KEYS[k] then
      res[k] = self[k]
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
  -- TODO: make async
  if self.morph then
    return self.morph:request(lubyk.get_url, url)
  else
    -- error
    return nil
  end
end

function lib:findClass(class_name)
  -- TODO: make async ?
  local code
  if self.morph then
    local path_name = string.gsub(class_name, '%.','/')
    code = self.morph:request(lubyk.get_url, '/lib/' .. path_name .. '.lua')
  end
  return code or lk.findCode(class_name)
end

--=========================================================== Process

local update_url,       dump_url,        quit_url = 
      lubyk.update_url, lubyk.dump_url,  lubyk.quit_url

--- Answering requests to Process.
function lib:callback(url, ...)
  if url == dump_url then
    -- sync call, return content
    return self:dump()
  elseif url == update_url then
    local data = ...
    -- async call, no return value
    self:set(data)
    local pdump = self:partialDump(data)
    self:notify(pdump)
  elseif url == quit_url then
    sched:quit()
  else
    -- Inter process communication
    local inlet = self:get(url)
    if inlet then
      inlet.receive(...)
    else
      self:error("Received calls on missing inlet '%s'", url)
    end
  end
end

function lib:error(...)
  -- TODO: notify error
  printf(...)
end

--- Inform listening editors that something changed.
function lib:notify(...)
  local service = self.service
  if service then
    service:notify(...)
  end

  for _, clbk in pairs(self.on_notify) do
    clbk(...)
  end
end

function lib:onNotify(key, callback)
  self.on_notify[key] = callback
end

-- Set a single parameter
function lib:setParam(lubyk, p, param_name, value)
  local node, err = self:findByPath(p)
  if node then
    node:setParam(param_name, value)
  else
    lubyk.log('error', err)
  end
end

-- Set a parameter from a relative url like "metro/@tempo".
-- FIXME: Is this used ?
function lib:control(url, value)
  local parts = lk.split(url, '/')
  local param_name = parts[#parts]
  local loc = self
  for i = 1,(#parts-1) do
    local part = parts[i]
    loc = loc.nodes[part]
    if not loc then
      return nil-- abort
    end
  end
  return loc:control(param_name, value)
end

--- In order to resolve cross-process links, return a process by a given name.
function lib:findProcess(name)
  if name == self.name then
    return self
  elseif self.process_watch then
    -- RemoteProcess
    return self.process_watch:process(name)
  else
    -- TODO: better error handling
    print(string.format("Cannot find process '%s' (no lk.ProcessWatch).", process_name))
    return nil
  end
end

--- Return the patches url. This is the same as the patch's name for non-nested
-- nodes.
function lib:url()
  return '/' .. self.name
end

--- We found the morph server, load patch
function lib:setMorph(process)
  if process then
    self.morph = process.req
    self:sync()
  else
    self.morph = nil
  end
end

--- Reload everything from morph
function lib:sync()
  local patch = self:findCode(self:url() .. '/_patch.yml')
  if patch then
    private.loadFromYaml(self, patch)
    self:notify(self:dump())
  else
    print("Could not sync: no _patch.yml")
  end
end

--=============================================== ProcessWatch delegate

-- Callback on found process in ProcessWatch. Not used for the moment.
function lib:processConnected(remote_process)
  if remote_process.name ~= '' then
    -- this is a process
  else
    -- this is the morph server
    self:setMorph(remote_process)
  end
end

-- Callback on removed process in ProcessWatch. Not used for the moment.
function lib:processDisconnected(remote_process)
  if remote_process.name ~= '' then
    -- process: noop
  else
    -- morph going offline
    self:setMorph(nil)
  end
end

--=============================================== PRIVATE

function private:loadFromYaml(yaml_code)
  local data = yaml.load(yaml_code) or {}
  -- clear before loading (yaml contains a full definition)
  self.nodes = {}
  self:set(data)
end

function private:setNodes(nodes_definition)
  local to_link = {}
  local nodes = self.nodes
  local to_remove = {}
  
  -- 1. Create/remove/update nodes
  for name, def in pairs(nodes_definition) do
    -- parsing each node
    local node = nodes[name]
    if not def then
      -- remove node
      if node then
        to_remove[name] = node
      end
    else
      if not node then
        -- create node
        node = lk.Node(self, name)
        nodes[name] = node
      end
      -- update
      node:set(def, true)
      local links = def.links
      if links then to_link[node] = links end
    end
  end
  
  -- 2. Update links later
  -- FIXME: This is an ugly hack and should not be necessary...
  local key = {}
  self[key] = lk.Thread(function()
    for node, links in pairs(to_link) do
      node:setLinks(links)
    end
    self[key] = nil

    -- 3. Remove nodes
    for name, node in pairs(to_remove) do
      node:remove()
      nodes[name] = nil
    end
  end)

end


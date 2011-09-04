--[[------------------------------------------------------

  lk.Morph
  --------

  This Service announces itself with "morph" type and
  "Morpheus" name. It's role is to provide save and restore
  functionality to a processing network.

  TODO: automatic versioning of saved changes (git)

  Usage: a morph process is started with a path to a lubyk
  project file (*.lkp, yaml content). The lkp file contains
  information on machine assignement of the processes.

--]]------------------------------------------------------

local lib = {type='lk.Morph'}
lib.__index = lib
lk.Morph    = lib

local processProxy   = {}
processProxy.__index = processProxy

setmetatable(lib, {
  -- new method
 __call = function(lib, opts)
  local self = {
    zone = opts.zone,
    -- Holds the list of all the processes that need to be running
    -- for the project to work (not just the once actually running).
    process_list = {},
  }
  setmetatable(self, lib)
  -- We need to watch for stem cells (or other ways to spawn new
  -- processes that are not local) and we need to know which processes
  -- are already running and which ones we need to create.
  --
  -- TODO: implement callbacks before enabling
  -- self.process_watch = lk.ProcessWatch():addDelegate(self)
  -- This is similar to a file open...
  local srv_opts = {
    callback = function(...)
      return self:callback(...)
    end,
  }

  self.service = lk.Service(self.zone .. ':', srv_opts)
  -- let service properly start.... ?
  -- FIXME: use a 'started' callback
  sleep(1000)
  self:openProject(opts.path)
  return self
end})

function lib:openProject(path)
  -- TODO: close currently opened project (stop remote processes)
  self.dir = lk.Dir(path)
  -- Find the list of processes. A process is a folder with a patch definition:
  -- .../foo/_patch.yml
  -- .../bar/_patch.yml
  self:startProcesses()
end

function lib:startProcesses()
  self.process_list = {}
  print('startProcesses', self.dir.path)
  for file in self.dir:glob('[^/]+/_patch.yml') do
    print('FILE', file)
    local basepath, _ = lk.directory(file)
    local _, basename = lk.directory(basepath)
    -- Read mapping (process_name --> hostname)...
    -- Spawn new process
    self:spawn('localhost', basename)
  end
  print('OK')
end

-- Spawn a new process that will callback to get data (yml definition, assets).
function lib:spawn(host_name, process_name)
  print('Starting', process_name)
  -- spawn Process
  -- We start a mimas.Application in case the process needs GUI elements.
  worker:spawn([[
  require 'lubyk'
  app     = mimas.Application()
  process = lk.Process(%s)
  app:exec()
  ]], {
    name = process_name,
    zone = self.zone,
  })
  -- the process will find the morph's ip/port by it's own service discovery
end

function lib:newProcess(name)
  -- Create a new process proxy
  local process = {name = name}
  setmetatable(process, processProxy)
  return process
end

local function write(self, url, data)
  print(string.format('======================================= %s\n%s\n=======================================', url, data))
end

local function setCode(self, code_cache, node_url, code)
  local url = self.name .. '/' .. node_url .. '.lua'
  if code_cache[url] == code then
    -- same, ignore
  elseif code then
    code_cache[url] = code
    write(self, url, code)
  else
    code_cache[url] = nil
  end
end

local function writePatch(self)
  local url = self.name .. '/' .. '_patch.yml'
  write(self, self.name .. '/' .. '_patch.yml', yaml.dump(self.cache))
end

local function getCache(self, remote_service)
  self.cache = self.req:request(lubyk.dump_url)
  local code_cache = {}
  self.code_cache = code_cache
  -- move code into code cache (so that it is not in the dump)
  local nodes = self.cache.nodes
  if nodes then
    for name, node in pairs(nodes) do
      setCode(self, code_cache, name, node.code)
      node.code = nil
    end
  end
  writePatch(self)
end

function processProxy:connect(remote_service)
  self.req   = remote_service.req
  getCache(self, remote_service)
  --======================================= SUB client
  self.sub = zmq.SimpleSub(function(changes)
    -- we receive notifications, update content
    self:write(changes)
  end)
  self.sub:connect(remote_service.sub_url)
end

function processProxy:disconnect()
  self.req = nil
  self.sub = nil
end

local function deepMerge(base, key, value)
  local base_v = base[key]
  if type(value) == 'table' then
    if not base_v then
      base[key] = value
      return true
    else
      -- merge
      local changed = false
      for k, v in pairs(value) do
        changed = deepMerge(base_v, k, v) or changed
      end
      return changed
    end
  elseif base_v == value then
    -- nothing changed
    return false
  else
    base[key] = value
    return true
  end
end

function processProxy:write(changes)
  local patch_changed = false
  local cache = self.cache
  local code_cache = self.code_cache
  for base_k, base_v in pairs(changes) do
    if base_k == 'nodes' then
      local nodes = base_v
      local cache_nodes = cache[base_k] or {}
      for name,node in pairs(nodes) do
        local cache_node = cache_nodes[name] or {}
        for k, v in pairs(node) do
          if k == 'code' then
            setCode(self, code_cache, name, v)
          else
            patch_changed = deepMerge(cache_node, k, v) or patch_changed
          end
        end
      end
    else
      patch_changed = deepMerge(cache, base_k, base_v) or patch_changed
    end
  end
  if patch_changed then
    writePatch(self)
  end
end

function lib:addProcess(process)
  -- do nothing
end

--- Return the content of the file at the given path in the
-- current project.
function lib:get(path)
  return lk.readall(self.dir.path .. '/' .. path)
end
--============================================= lk.Service delegate

--- Answering requests to Morph.
function lib:callback(url, ...)
  if url == lubyk.dump_url then
    return {
      zone = self.zone,
    }
  elseif url == lubyk.update_url then
    local data = ...
    -- async call, no return value
    -- update state
  elseif url == lubyk.get_url then
    return self:get(...)
  else
    -- ignore
    print('Bad message to lk.Morph', url)
  end
end

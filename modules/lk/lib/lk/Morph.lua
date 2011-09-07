--[[------------------------------------------------------

  lk.Morph
  --------

  This Service announces itself with "morph" type and
  "Morpheus" name. It's role is to provide save and restore
  functionality to a processing network.

  The Morph server relies on the Lubyk.zone global setting.

  TODO: automatic versioning of saved changes (git)

  Usage: a morph process is started with a path to a lubyk
  project file (*.lkp, yaml content). The lkp file contains
  information on machine assignement of the processes.

--]]------------------------------------------------------

local lib = {type='lk.Morph'}
lib.__index = lib
lk.Morph    = lib

local private = {}

setmetatable(lib, {
  -- new method
 __call = function(lib, filepath)
  local self = {
    -- Version of lubyk used to create project
    lubyk     = {version = Lubyk.version},
    -- Holds the list of all the processes that need to be running
    -- for the project to work (not just the ones actually running).
    processes = {},
  }
  setmetatable(self, lib)

  private.start(self)
  if filepath then
    self:openFile(filepath)
  end
  return self
end})

function lib:openFile(filepath)
  self:close()
  self.dir = lk.directory(filepath)
  self.path = filepath
  private.readFile(self)
end

function lib:close()
  -- TODO: close all processes in same zone
  -- clear
  self.processes = {}
  self.path = nil
  self.dir  = nil
  self.resources = {}
end

--- Return the content of the file at the given path in the
-- current project.
function lib:get(path)
  local resource = self.resources[path]
  if not resource then
    resource = lk.FileResource(self.dir .. '/' .. path)
    self.resources[path] = resource
  end
  return resource.body
end
--============================================= lk.Service delegate

--- Answering requests to Morph.
function lib:callback(url, ...)
  if url == lubyk.dump_url then
    return private.dumpAll(self)
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

--=============================================== lk.ProcessWatch delegate

--- When services are brought online
function lib:processConnected(process)
  -- We need to receive notifications from this process so that
  -- we can write the changes to file.
  local process = self.processes[process.name]
  if process then
    -- we are interested in this process
    private.process.connect(self, process)
  end
end

function lib:processDisconnected(process)
  local process = self.processes[process.name]
  if process then
    private.process.disconnect(self, process)
  end
end

--=============================================== PRIVATE
local DUMP_KEYS = {'lubyk', 'processes'}
private.dump = {}
private.set  = {}
private.process = {}

--- Start service and launch processes.
function private:start(process_watch)
  process_watch = process_watch or lk.ProcessWatch()
  self.process_watch = process_watch:addDelegate(self)
  local srv_opts = {
    callback = function(...)
      return self:callback(...)
    end,
  }
  -- TODO: make sure we are the only morph server in this zone (registration
  -- name is not 'zone:-1'...
  self.service = lk.Service(Lubyk.zone .. ':', srv_opts)
end


-- Reads and parses the content of the lkp file. This creates the file
-- if it does not exist.
function private:readFile()
  local path = self.path
  if not lk.exist(path) then
    private.writeFile(self)
  else
    local def = yaml.loadpath(self.path)
    local h = private.set
    for _, k in ipairs(DUMP_KEYS) do
      -- private.set.lubyk(self, def.lubyk)
      h[k](self, def[k])
    end
  end
end

function private:dumpAll()
  local dump = {}
  local h = private.dump
  for _, k in ipairs(DUMP_KEYS) do
    h[k](self, dump)
  end
  return dump
end

function private:writeFile()
  lk.writeall(self.path, yaml.dump(private.dumpAll(self)))
end

function private.set:lubyk(lubyk)
  if lubyk and lubyk.version then
    self.lubyk = lubyk
  else  
    self.lubyk = {version = Lubyk.version}
  end
end

function private.dump:lubyk(dump)
  dump.lubyk = self.lubyk
end

function private.set:processes(processes)
  for name, process_info in pairs(processes or {}) do
    if process_info == '' then
      process_info = {}
    end
    private.process.add(self, name, process_info, true)
  end
end

function private.dump:processes(dump)
  local processes = {}
  dump.processes = processes
  for name, process in pairs(self.processes) do
    local p, empty = {}, true
    if process.host ~= 'localhost' then
      p.host = process.host
      empty = false
    end
    if process.dir ~= name then
      p.dir = process.dir
      empty = false
    end
    if empty then
      processes[name] = ''
    else
      processes[name] = p
    end
  end
end

--=============================================== TODO: cleanup, fix

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

--=============================================== PRIVATE process

function private.process:connect(process)
  local p = {process = process, cache = {}, code_cache = {}}
  process.sub = zmq.SimpleSub(function(changes)
    -- we receive notifications, update content
    private.process.changed(self, process, changes)
  end)
  process.sub:connect(process.sub_url)
end

function private.process:disconnect(process)
  self.process.sub = nil
end

function private.process.changed(self, process, changes)
  if true then
    print("private.process.changed TODO")
    return
  end
  -- write changes to file    
  -- local cache = p.cache
  -- local code_cache = self.code_cache
  -- for base_k, base_v in pairs(changes) do
  --   if base_k == 'nodes' then
  --     local nodes = base_v
  --     local cache_nodes = cache[base_k] or {}
  --     for name,node in pairs(nodes) do
  --       local cache_node = cache_nodes[name] or {}
  --       for k, v in pairs(node) do
  --         if k == 'code' then
  --           setCode(self, code_cache, name, v)
  --         else
  --           patch_changed = deepMerge(cache_node, k, v) or patch_changed
  --         end
  --       end
  --     end
  --   else
  --     patch_changed = deepMerge(cache, base_k, base_v) or patch_changed
  --   end
  -- end
  -- if patch_changed then
  --   writePatch(self)
  -- end                                         
end

function private.process.readFile(process)
  if lk.exist(process.patchpath) then
    process.cache = yaml.loadpath(process.patchpath)
  else
    process.cache = {}
  end
end

function private.process.writeFile(process)
  lk.writeall(process.patchpath, yaml.dump(process.nodes))
end

--=============================================== add/remove process

-- When reading a file 'reading_lkp' is set so we know that we must not
-- write to lkp file.
function private.process.add(self, name, info, reading_lkp)
  local process = self.process_watch:process(name)
  self.processes[name] = process
  process.host = info.host or 'localhost'
  process.dir  = info.dir  or name
  process.absdir = self.dir .. '/' .. process.dir
  lk.makePath(process.absdir)
  process.patchpath = process.absdir .. '/_patch.yml'
  private.process.readFile(process)
  private.process.start(self, process)
  if not reading_lkp then
    -- This is a new process
    private.writeFile(self)
    private.process.writeFile(process)
  end
end

function private.process:start(process)
  assert(not process.online)
  -- Read mapping (process_name --> hostname)...
  if process.host == Lubyk.host or process.host == 'localhost' then
    -- Spawn new process
    self:spawn(process.name)
  else
    -- TODO: find/wait for remote host
    -- local stem = self.process_watch:stem(process.host)
    -- stem:spawn(process.name) [ will be done on connect ]
  end
end

-- Spawn a new process that will callback to get data (yml definition, assets).
-- This is private but we need to modify it during testing so we
-- make it accessible from 'self'.
function lib.spawn(self, name)
  -- spawn Process
  -- We start a mimas.Application in case the process needs GUI elements.
  worker:spawn([[
  require 'lubyk'
  app     = mimas.Application()
  process = lk.Process(%s)
  app:exec()
  ]], name)
  -- TODO: We might make things faster by giving the ip/port stuff to the
  -- process...
  -- the process will find the morph's ip/port by it's own service discovery
end

-- We need this for testing
lib.private = private

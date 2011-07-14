--[[------------------------------------------------------

  lk.Morph
  --------

  This Service announces itself with "morph" type and
  "Morpheus" name. It's role is to provide save and restore
  functionality to a processing network.

--]]------------------------------------------------------

local lib = {type='lk.Morph'}
lib.__index = lib
lk.Morph    = lib

local processProxy   = {}
processProxy.__index = processProxy

setmetatable(lib, {
  -- new method
 __call = function(lib)
  local self = {}
  setmetatable(self, lib)
  self.process_watch = lk.ProcessWatch(self, function(...) return self:newProcess(...) end)
  return self
end})

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
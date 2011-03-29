--[[------------------------------------------------------

  lk.Node
  -------

  Encapsulate lua code (protect global space, enable reload,
  provide links and introspection).

--]]------------------------------------------------------
-- node metatable
local lib   = {type='lk.Node'}
lib.__index = lib
lk.Node     = lib

-- node's environment metatable
local env_mt= {}

setmetatable(lib, {
  -- new method
 __call = function(table, process, name, code_str)
  -- new environment
  local env  = {}
  -- new node
  local instance = {
    inlets         = {},
    outlets        = {},
    env            = env,
    errors         = {},
    name           = name,
    process        = process,
  }
  setmetatable(instance, lib)
  -- method to declare inlets
  env.inlet  = lk.InletMethod(instance)
  -- method to declare outlets
  env.outlet = lk.OutletMethod(instance)
  -- env has read access to _G
  setmetatable(env, env_mt)

  process.nodes[name] = instance
  -- pending connection resolution
  instance.pending_inlets = process.pending_nodes[name] or {}
  process.pending_nodes[name] = nil
  if code_str then
    instance:eval(code_str)
  end
  return instance
end})

-- metatable for the new global env in each
-- node
function env_mt.__index(env, name)
  -- if the variable exists in original _G,
  -- cache locally
  local gvar = _G[name]
  rawset(env, name, gvar)
  return gvar
end

-- function to reload code
function lib:eval(code_str)
  local code = assert(loadstring(code_str))
  -- code will execute in node's environment
  setfenv(code, self.env)
  code()
end

function lib:set(definition)
  -- load params before evaluating script code
  local params = definition.params
  if params then
    self:setParams(params)
  end

  -- should protect with pcall to avoid breaking all if it fails
  if definition.class then
    -- find source in package.paths
    local code = lk.findcode(definition.class)
    if code then
      self:eval(code)
    else
      -- FIXME: set error on Node
      error(string.format("Could not find source code for '%s'.", definition.class))
    end
  elseif definition.script then
    self:eval(definition.script)
  end

  for k, v in pairs(definition) do
    if k == 'source' or
       k == 'params' or
       k == 'links' then
      -- ignore (we want to run these in a specific order)
    else
      self[k] = v
    end
  end

  local links = definition.links
  if links then
    self:setLinks(links)
  end
end

function lib:setParams(params)
  local env = self.env
  for k, v in pairs(params) do
    env[k] = v
  end
end

function lib:error(...)
  print(string.format(...))
--  table.insert(self.errors, string.format(...))
end

local function setLink(self, out_name, target_url, process)
  local outlet = self.outlets[out_name]
  if not outlet then
    self:error("Outlet name '%s' does not exist.", out_name)
  else
    local slot, err = process:get(target_url, lk.Inlet)
    if slot == false then
      -- error
      self:error(err)
    elseif not slot then
      -- slot not found
      -- If the slot does not exist yet, make a draft to be used
      -- when the node creates the real inlet.
      slot, err = process:pendingInlet(target_url)
      if not slot then
        -- FIXME: store absolute path for 'target_url' in process pending list
        -- and resolve this list on node creation
        self:error(err)
        return
      end
    end
    -- connect to real or pending slot
    outlet:connect(slot)
  end
end

function lib:setLinks(links)
  local process = self.process
  for out_name, def in pairs(links) do
    if type(def) == 'table' then
      -- multiple links
      for _, ldef in ipairs(def) do
        setLink(self, out_name, ldef, process)
      end
    else
      setLink(self, out_name, def, process)
    end
  end
end
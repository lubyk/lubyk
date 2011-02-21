--[[------------------------------------------------------

  lk.Node
  -------

  Encapsulate lua code (protect global space, enable reload,
  provide links and introspection).

--]]------------------------------------------------------
-- node metatable
local lib   = {}
lib.__index = lib
lk.Node     = lib

-- node's environment metatable
local env_mt= {}

setmetatable(lib, {
  -- new method
 __call = function(table, code_str)
  -- new environment
  local env  = {}
  -- new node
  local node = {inlets={}, outlets={}, env=env}
  setmetatable(node, lib)
  -- method to declare inlets
  env.inlet  = lk.InletMethod(node)
  -- method to declare outlets
  env.outlet = lk.OutletMethod(node)
  -- env has read access to _G
  setmetatable(env, env_mt)

  node:eval(code_str)
  return node
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

function lib:set(params)
  local env = self.env
  for k, v in pairs(params) do
    env[k] = v
  end
end
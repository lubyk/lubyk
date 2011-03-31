--[[------------------------------------------------------

  lk.Environment
  --------------

  Run code in a new environment with read-only access to the
  global namespace.

--]]------------------------------------------------------

local lib = {type='lk.Environment'}
lk.Environment = lib

setmetatable(lib, {
  -- new method
 __call = function(lib, code_str)
  local code = assert(loadstring(code_str))
  -- new environment
  local env  = {}
  setmetatable(env, lib)
  setfenv(code, env)
  return code
end})

-- metatable for the new global env in each
-- node
function lib.__index(env, name)
  -- if the variable exists in original _G,
  -- cache locally
  local gvar = _G[name]
  rawset(env, name, gvar)
  return gvar
end

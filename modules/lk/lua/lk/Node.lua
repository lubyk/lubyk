--[[------------------------------------------------------

  lk.Node
  -------

  Encapsulate lua code (protect global space, enable reload,
  provide links and introspection).

--]]------------------------------------------------------
local lib = {}
lk.Node   = lib

setmetatable(lib, {
  -- new method
 __call = function(table, code_str)
  code_str = 'node = node or lk.Node()'
  local code = assert(loadstring(code_str))
  -- new environment
  local env  = {}
  setmetatable(env, lib)
  setfenv(code, env)
  return code()
end})

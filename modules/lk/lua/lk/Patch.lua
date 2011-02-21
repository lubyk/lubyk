--[[------------------------------------------------------

  lk.Patch
  --------

  A patch provides introspection and edit/restore features
  for nodes.

--]]------------------------------------------------------
local lib   = {}
lib.__index = lib
lk.Patch    = lib

setmetatable(lib, {
  -- new method
 __call = function(table)
  local instance = {}
  setmetatable(instance, lib)
  return instance
end})



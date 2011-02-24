--[[------------------------------------------------------

  lk.OutletMethod
  ---------------

  Creates the outlet declaration method 'outlet' used in
  nodes.

--]]------------------------------------------------------
-- 'outlet' accessor metamethod
local lib = {type='lk.OutletMethod'}
lib.__index     = lib
lk.OutletMethod = lib

setmetatable(lib, {
  -- new method
 __call = function(table, node)
  -- Create outlet() method/accessor for a given node
  local instance = {node = node}
  setmetatable(instance, lib)
  return instance
end})

function lib.__call(self, ...)
  -- outlet(name, info)
  -- Declare a new outlet
  return lk.Outlet(self.node, ...)
end

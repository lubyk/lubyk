--[[------------------------------------------------------

  lk.OutletMethod
  ---------------

  Creates the outlet declaration method 'outlet' used in
  nodes.

--]]------------------------------------------------------
-- 'outlet' accessor metamethod
local lib = {}

function lk.OutletMethod(node)
  -- Create inlet() method/accessor for a given node
  local instance = {node = node}
  setmetatable(instance, lib)
  return instance
end

function lib.__call(self, ...)
  -- outlet(name, info)
  -- Declare a new outlet
  return lk.Outlet(self.node, ...)
end

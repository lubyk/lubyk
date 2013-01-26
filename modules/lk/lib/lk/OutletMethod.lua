--[[------------------------------------------------------

  lk.OutletMethod
  ---------------

  Creates the outlet declaration method 'outlet' used in
  nodes.

--]]------------------------------------------------------
-- 'outlet' accessor metamethod
local lib = {type='lk.OutletMethod'}
lk.OutletMethod = lib

setmetatable(lib, {
  -- new method
 __call = function(lib, node)
  -- Create outlet() method/accessor for a given node
  local self = {node = node}
  return setmetatable(self, lib)
end})

function lib:__call(name, connect_msg, disconnect_msg)
  local node = self.node
  -- Declare or update an outlet.
  local outlet = node.outlet_by_name[name]
  if not outlet then
    outlet = lk.Outlet(self.node, name, connect_msg, disconnect_msg)
    node.outlet_by_name[name] = outlet
  end
  if opts then
    outlet:set(opts)
  end
  -- Ordered list of outlets, GC protected.
  table.insert(node.slots.outlets, outlet)
  node.env[name] = outlet.send
  return outlet
end

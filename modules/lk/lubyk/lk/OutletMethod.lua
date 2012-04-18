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

function lib:__call(name, opts)
  local node = self.node
  -- Declare or update an outlet.
  local oultet = node.outlets[name]
  if not oultet then
    outlet = lk.Outlet(name, self.node)
    node.outlets[name] = outlet
  end
  if opts then
    outlet:set(opts)
  end
  table.insert(node.slots.outlets, outlet)
  node.env[name] = outlet.send
  return outlet
end

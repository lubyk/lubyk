--[[------------------------------------------------------

  lk.InletMethod
  --------------

  Creates the inlet declaration method 'inlet' used in
  nodes.

--]]------------------------------------------------------
-- 'inlet' accessor metamethod
local lib = {type='lk.InletMethod'}
lib.__index    = lib
lk.InletMethod = lib

setmetatable(lib, {
  -- new method
 __call = function(lib, node)
  -- Create inlet() method/accessor for a given node
  local instance = {node = node}
  setmetatable(instance, lib)
  return instance
end})

function lib.__newindex(self, name, func)
  -- inlet.foo(xxx)
  -- Set an inlet callback
  local node = assert(self.node)
  local inlet = node.inlets[name]
  if not inlet then
    inlet = node.pending_inlets[name]
    if inlet then
      node.pending_inlets[name] = nil
      -- Not pending anymore
      inlet:setNode(node)
    else
      -- New inlet
      inlet = lk.Inlet(node, name)
    end
    node.inlets[name] = inlet
  end
  -- Each time we redeclare an inlet, we add it.
  table.insert(node.slots.inlets, inlet)

  -- Protection with pcall
  inlet.receive = function(...)
    local ok, err = pcall(func, ...)
    if not ok then
      self.node:error(err)
    end
  end
end

--[[------------------------------------------------------

  lk.OutletMethod
  ---------------

  Creates the 'lubyk.o' declaration table used in nodes.

--]]------------------------------------------------------
local lib = {type='lk.OutletMethod'}
lib.__index     = lib
lk.OutletMethod = lib

setmetatable(lib, {__call = function(lib, ...) return lib.new(...) end})

function lib.new(node)
  -- Create lubyk.o declaration/accessor table for a given node
  local self = {_node = node}
  return setmetatable(self, lib)
end

function lib:__newindex(name, def)
  local node = self._node
  -- Declare or update an outlet.
  local outlet = node.outlets[name]
  if not outlet then
    outlet = lk.Outlet(node, name)
    node.outlets[name] = outlet
  end
  
  outlet:set(def)

  -- Ordered list of outlets, GC protected.
  table.insert(node.slots.outlets, outlet)
  rawset(self, name, outlet.send)
end

function lib:clear()
  for k,_ in pairs(self) do
    if k ~= '_node' then
      self[k] = nil
    end
  end
end

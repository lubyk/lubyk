--[[------------------------------------------------------

  lk.Inlet
  --------

  Incoming node slot.

--]]------------------------------------------------------
local lib   = {type='lk.Inlet'}
lib.__index = lib
lk.Inlet    = lib

---------------------------------------------------------- PRIVATE

---------------------------------------------------------- PUBLIC
setmetatable(lib, {
  -- lk.Inlet(node)
  -- Create a new inlet
 __call = function(table, node, name, ...)
  if type(node) == 'string' then
    name = node
    -- pending inlet
    local instance = {name = name}
    setmetatable(instance, lib)
    return instance
  else
    local instance = node.inlets[name]
    if not instance then
      instance = node.pending_inlets[name]
      if instance then
        node.pending_inlets[name] = nil
      else
        instance = {}
        setmetatable(instance, lib)
      end
      -- set node
      instance.node = node
      node.inlets[name] = instance
    end
    instance:set(name, ...)
    return instance
  end
end})

function lib:set(name, info)
  self.name = name
  self.info = info
end

function lib.receive(...)
  print('Inlet receive function not set.')
end

function lib:dump()
  return {
    name = self.name,
    info = self.info
  }
end

function lib:url()
  return self.node.name .. '/in/' .. self.name
end
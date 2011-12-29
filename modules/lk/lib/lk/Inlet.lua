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
 __call = function(lib, node, name, ...)
  local self
  if type(node) == 'string' then
    self = {name = node}
    -- pending inlet
    setmetatable(self, lib)
  else
    self = node.inlets[name]
    if not self then
      self = node.pending_inlets[name]
      if self then
        node.pending_inlets[name] = nil
      else
        self = {name = name}
        setmetatable(self, lib)
      end
      -- set node
      self.node = node
      node.inlets[name] = self
    end
    table.insert(node.sorted_inlets, self)
    self:set(name, ...)
  end

  if not self.receive then
    -- temporary dummy method
    function self.receive(...)
      print(string.format("'function inlet.%s' not defined.", self.name))
    end
  end
  return self
end})

function lib:set(name, info)
  self.name = name
  self.info = info
end

function lib.receive(...)
  print('Inlet function not set.')
end

function lib:dump()
  return {
    name = self.name,
    info = self.info
  }
end

function lib:url()
  if self.target_url then
    -- remote
    return self.target_url
  else
    -- local
    return self.node:url() .. '/in/' .. self.name
  end
end

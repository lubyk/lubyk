dofile('Outlet.lua')
Inlet = {}

function Inlet:accessor(inlet, val)
  local self = inlet.node
  if val then
    -- receiving value
    self[inlet.name] = val
  end
  return self[inlet.name]
end
-- TODO: rename Node to Prototype ?
Node = {}
Node.__index = Node

function Node.new()
  -- create a new node prototype
  local node = {inlets = {}, _outlets = {}}
  setmetatable(node, Node)
  return node
end

function Node:access(name, val)
  if val then
    -- receiving value
    self[name] = val
  end
  return self[name]
end

function Node:notify(name, val)
  print(string.format("[%s] %s --> '%s'", self.name, name, val))
end

-- register outlet in prototype.
function Node:outlet(name)
  local outlet = self._outlets[name]
  if not outlet then
    self._outlets[name] = {} -- outlet definition
  else
    -- merge outlet definition
  end
end

-- send to an outlet, lazyly creating outlets
function Node:send(name, val)
  self.outlets[name]:send(val)
end

function Node:inlet(name)
  local inlet = self.inlets[name]
  if not inlet then
    inlet = {name = name, node = self}
    inlet.call = function(node, val)
      return Node.access(node, name, val)
    end
    self.inlets[name] = inlet
  end
  return inlet
end

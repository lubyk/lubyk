Inlet = {}

function Inlet:accessor(inlet, val)
  local self = inlet.node
  if val then
    -- receiving value
    self[inlet.name] = val
  end
  return self[inlet.name]
end

Node = {}
Node.__index = Node

Node.new = function()
  -- create a new node instance
  local node = {inlets = {}, outlets = {}}
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

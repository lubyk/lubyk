Outlet = {}
Outlet.__index = Outlet

function Outlet.new(name, outlet_def)
  -- create a new Outlet instance
  local outlet = {name = name, connections = {}, definition = outlet_def}
  setmetatable(outlet, Outlet)
  return outlet
end

function Outlet:send(val)
  for i, con in ipairs(self.connections) do
    -- con[2] = method
    -- con[1] = obj
    con[2](con[1], val)
  end
end

function Outlet:connect(remote, method)
  for i, con in ipairs(self.connections) do
    if con[1] == remote and con[2] == method then
      -- do not create same link twice
      return
    end
  end
  table.insert(self.connections, {remote, method})
end



--- ================================== OutletList (lazy instanciation of outlets)
OutletList = {}

function OutletList.new(node, prototypes)
  local outlets = {node = node, prototypes = prototypes}
  setmetatable(outlets, OutletList)
  return outlets
end

function OutletList.__index(outlets, name)
  local outlet_def = outlets.prototypes[name]
  if outlet_def then
    local outlet = Outlet.new(name, outlet_def) -- pass outlet_def ?
    outlets[name] = outlet
    return outlet
  end
  return error(string.format("No outlet named '%s' in object %s.", name, outlets.node.name))
end
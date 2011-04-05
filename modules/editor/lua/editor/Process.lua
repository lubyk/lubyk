--[[------------------------------------------------------

  editor.Process
  --------------

  The editor.Process manages the connection with the
  remote Process and holds information on the state of the
  remote. This is like lk.Client but with a single
  subscription.

--]]------------------------------------------------------

local lib      = {type='editor.Process'}
lib.__index    = lib
editor.Process = lib

setmetatable(lib, {
  -- new method
 __call = function(lib, remote_service, delegate)
  local instance = {
    name           = remote_service.name,
    x              = 100,
    y              = 100,
    -- The delegate is used by views.
    delegate       = delegate,
    push           = remote_service.push,
    req            = remote_service.req,
    nodes          = {},
    pending_inlets = {},
  }

  instance.hue = remote_service.info.hue or 0.5

  --======================================= SUB client
  instance.sub = zmq.SimpleSub(function(changes)
    -- we receive notifications, update content
    instance:set(changes)
  end)
  instance.sub:connect(remote_service.sub_url)

  setmetatable(instance, lib)

  instance:sync()

  return instance
end})

--- Nodes in patch changed, store change and update view if needed.
-- To remove a node, set it's value to false.
local function setNodes(self, nodes_def)
  local nodes = self.nodes
  for node_name, node_def in pairs(nodes_def) do
    local node = nodes[node_name]
    if node then
      if node_def then
        -- set existing node
        node:set(node_def)
      else
        -- remove node
        node:remove()
        nodes[node_name] = nil
      end
    elseif node_def then
      -- create new node
      node = editor.Node(self, node_name, node_def)
      nodes[node_name] = node
    end
  end
end

local function doSet(self, definition)
  for k, v in pairs(definition) do
    if k == 'name' then
      self.name = v
      if self.view then
        self.view:setName(v)
      end
    elseif k == 'nodes' then
      setNodes(self, v)
    else
      self[k] = v
    end
  end
end

-- Synchronize with remote process.
function lib:sync()
  local definition = self.req:request(lubyk.dump_url)
  --print(yaml.dump(definition))
  self:set(definition)
end

--- Change remote content.
function lib:change(definition)
  self.push:send(lubyk.update_url, definition)
end

-- If self.view is nil, only set the data without
-- creating/changing views.
function lib:set(definition)
  if self.view then
    app:post(function()
      doSet(self, definition)
      self:updateView()
    end)
    self.view:processChanged()
  else
    doSet(self, definition)
  end
end

-- When the ProcessView is created, it triggers this method
-- to build/update views
function lib:updateView()
  for _,node in pairs(self.nodes) do
    node:updateView()
  end

  for _,node in pairs(self.nodes) do
    for _,outlet in ipairs(node.outlets) do
      for _,link in pairs(outlet.links) do
        link:updateView()
      end
    end
  end

  self.view:processChanged()
end

-- find a node in the current process (same as lk.Patch.get).
lib.get = lk.Patch.get

-- Create a pending inlet (same as lk.Patch.pendingInlet).

-- Create a pending inlet.
function lib:pendingInlet(inlet_url)
  local parts = lk.split(inlet_url, '/')
  local node_name, inlet_name
  if #parts == 3 and parts[2] == 'in' then
    node_name, inlet_name = parts[1], parts[3]
  else
    -- FIXME: store absolute path for 'in_url' in process pending list
    -- and resolve this list on node creation
    return nil, string.format("Invalid link url '%s' (target not found and cannot create temporary).", in_url)
  end

  local node = self.nodes[node_name]
  local inlet = nil
  if node then
    -- Node exists but inlet is not created yet.
    -- inlet = editor.Inlet(inlet_name)
    -- node.pending_inlets[inlet_name] = inlet
    return nil, string.format("Unknown inlet '%s' in node '%s'", inlet_name, node_name)
  else
    -- node not created yet
    local pending_inlets = self.pending_inlets[node_name]
    if not pending_inlets then
      pending_inlets = {}
      self.pending_inlets[node_name] = pending_inlets
    end
    inlet = pending_inlets[inlet_name]
    if not inlet then
      inlet = editor.Inlet(pending_inlets, inlet_name)
    end
  end
  return inlet
end

--- Display ProcessView in the PatchingView.
-- Should be called from withing GUI thread.
function lib:toggleView()
  self.delegate:toggleView(self)
end

function lib:deleteView()
  -- TODO: why don't we use Qt signals and slots ?
  for k, node in pairs(self.nodes) do
    node:deleteView()
  end

  if self.view then
    self.view:delete()
    self.view = nil
  end
end

--- Make a new node in the remote process with the given
-- definition.
function lib:newNode(definition)
  definition.code = definition.code or [[
inlet('input', 'Information on input [type].')
output = outlet('output', 'Information on output [type].')

function inlet.input(val)
  -- print and pass through
  print(val)
  output(val)
end
]]
  self:change {nodes = {[definition.name or 'new node'] = definition}}
end

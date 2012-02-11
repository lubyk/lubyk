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

local private  = {}

setmetatable(lib, {
  -- new method
 __call = function(lib, name, zone)
  local self = {
    name             = name,
    zone             = zone,
    x                = 100,
    y                = 100,
    nodes            = {},
    pending_inlets   = {},
    -- List of nodes not notified by process but needed by links
    -- or controls.
    pending_nodes    = {},
  }

  setmetatable(self, lib)
  return self
end})

--- Nodes in patch changed, store change and update view if needed.
-- To remove a node, set it's value to false.
local function setNodes(self, nodes_def)
  local nodes = self.nodes
  local pending_nodes = self.pending_nodes
  for node_name, node_def in pairs(nodes_def) do
    local node = nodes[node_name]
    if node then
      if node_def then
        -- set existing node
        node:set(node_def)
      else
        -- remove node
        node:delete()
        nodes[node_name] = nil
      end
    elseif node_def then
      -- create new node
      node = pending_nodes[node_name]
      if node then
        pending_nodes[node_name] = nil
        node:set(node_def)
      else
        node = editor.Node(self, node_name, node_def)
      end
      node:connect()
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
    elseif k == 'hue' then
      self[k] = v
      if self.tab then
        self.tab:setHue(v)
      end
      if self.view then
        self.view:setHue(v)
      end
    else
      self[k] = v
    end
  end
end

-- Synchronize with remote process.
function lib:sync()
  local definition = self.req:request(lubyk.dump_url)
  self:set(definition)
end

--- Change remote content.
function lib:change(definition)
  self.push:send(lubyk.update_url, definition)
end

--- Restart process.
function lib:restart()
  local view = self.view
  if view then
    view:animate(8000)

    -- Remove ghost on process connection
    self.zone:onProcessDisconnected(name, function()
      if view.thread then
        view.thread:kill()
      end
      view.thread = nil
    end)
  end

  --self.push:send(lubyk.execute_url, 'restart')
end

--- Remove (delete) process from patch.
function lib:remove(skip_morph)
  self.known_to_morph = false
  local view = self.view
  if view then
    view:animate(8000)

    -- Remove ghost on process connection
    self.zone:onProcessDisconnected(name, function()
      if view.thread then
        view.thread:kill()
      end
      view.thread = nil
    end)
  end

  if not skip_morph then
    -- This remove operation is not part of a morph changed notification.
    self.zone.morph:removeProcess(self)
  end
end

-- If self.view is nil, only set the data without
-- creating/changing views.
function lib:set(definition)
  doSet(self, definition)
  if self.view then
    self.view:processChanged()
    self:updateView()
  end
end

-- When the ProcessView is created, it triggers this method
-- to build/update views
function lib:updateView()
  self:redrawViews()

  self.view:processChanged()
end

--- Used when the process is dragged
function lib:redrawViews()
  for _,node in pairs(self.nodes) do
    node:updateView()
    for _,outlet in ipairs(node.outlets) do
      for _,link in pairs(outlet.links) do
        link:updateView()
      end
    end
  end
end

--- Find a node from a given url (same as lk.Patch.get).
lib.get = lk.Patch.get

--- Find a node from a path (same as lk.Patch.findByPath).
lib.findByPath = lk.Patch.findByPath

--- Find a process by name.
function lib:findProcess(process_name)
  if process_name == self.name then
    return self
  elseif self.zone then
    -- ask editor.Zone for process
    return self.zone:findProcess(process_name)
  else
    -- TODO: better error handling
    print(string.format("Cannot find process '%s' (no lk.ProcessWatch).", process_name))
    return nil
  end
end

-- Create a pending inlet from an url relative to this process (nearly the same
-- as lk.Patch.pendingInlet).
function lib:pendingInlet(inlet_url)
  -- inlet_url example:
  --   node/in/slot
  local parts = lk.split(inlet_url, '/')
  local node_name, inlet_name
  if #parts == 3 and parts[2] == 'in' then
    node_name, inlet_name = parts[1], parts[3]
  else
    return nil, string.format("Invalid pendingInlet url '%s'.", inlet_url)
  end

  local node = self.nodes[node_name]
  local inlet = nil
  if node then
    -- Node exists but inlet is not created yet.
    inlet = editor.Inlet(node.pending_inlets, inlet_name)
    node.pending_inlets[inlet_name] = inlet
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
      pending_inlets[inlet_name] = inlet
    end
  end
  return inlet
end

--- Display ProcessView in the PatchView.
function lib:toggleView()
  self.zone:toggleView(self)
end

function lib:deleteView()
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
  if not definition.class then
    definition.code = definition.code or [=[
--[[
inlet('input', 'Information on input [type].')
outlet('output', 'Information on output [type].')

function inlet.input(val)
  -- print and pass through
  print(val)
  output(val)
end
--]]


]=]
  end
  self:change {nodes = {[definition.name or 'new node'] = definition}}
end

-- Process is coming online.
function lib:connect(remote_process, zone)
  self.zone = zone
  self.push = remote_process.push
  self.req  = remote_process.req

  self.hue = remote_process.info.hue or 0.5

  --======================================= SUB client
  self.sub = zmq.SimpleSub(function(...)
    -- we receive notifications, update content
    self:set(...)
  end)
  self.sub:connect(remote_process.sub_url)

  self:sync()

  self.online = true
  for _, node in pairs(self.nodes) do
    node:connect()
  end

  if self.tab then
    self.tab:setHue(self.hue)
  end
  -- editor.Process needed by ProcessTab
  remote_process.process = self
  zone:toggleView(self)
end

function lib:connected()
  return self.sub and true
end

-- A remote process is going offline: disconnect all links to this dying
-- process and mark them as pending.
function lib:disconnectProcess(remote_process)
  -- Disconnect all linked inlets
  for _, node in pairs(self.nodes) do
    node:disconnectProcess(remote_process)
  end
end

-- Our remote process is dying.
function lib:disconnect()
  -- disconnect self.sub ?
  self.sub = nil
  self.online = false
  if self.tab then
    -- mark as offline
    self.tab:setHue(self.hue)
  end
  for _, node in pairs(self.nodes) do
    node:disconnect()
  end
end

function lib:url()
  return '/' .. self.name
end

-- Returns an existing editor.Node or creates a pending node
function lib:findNode(node_name)
  local node = self.nodes[node_name]
  if not node then
    node = editor.Node(self, node_name, {})
    self.pending_nodes[node_name] = node
    node.online = false
  end
  return node
end

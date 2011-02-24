--[[------------------------------------------------------

  lk.Properties
  -------------

  This enables state storage with notifications. The
  properties act like both inlets and outlets. As an inlet,
  the property can be given a callback triggering a private
  function. As an outlet, any external node can register
  a callback function.

  Properties are stored in the "prop.properties" container,
  the values are in "prop.values".

  To change a property (that triggers all callbacks), you
  need to use prop.foo = 'xxx'. Reading is done with
  prop.foo. If you need to change the value without triggering
  the callbacks, you can use prop.values[foo] = 'xxx'.

--]]------------------------------------------------------
local lib = {type='lk.Properties'}
lk.Properties = lib

-- node.property('foo', 'info message')
local function node_declare_property(node, prop_name, info)
  local self = node.prop
  local prop = self.properties[prop_name]
  if not prop then
    -- declaring a new property (not an update)
    prop = {name = prop_name, observers = {}}
    self.properties[prop_name] = prop
  end
  -- only thing that an be updated
  prop.info = info
end

-- property accessor with callbacks
local function set_property(self, idx, value)
  local prop = self.properties[idx]
  if not prop then
    error(string.format('Unknown property %s', idx))
  end
  local old_value = self.values[idx]
  local callback  = self.node[prop.name]
  if callback then
    value = callback(self.node, value)
  end
  if value ~= old_value then
    -- set new value
    self.values[idx] = value
    -- notify value change
    local observers = prop.observers
    for _, list in ipairs(observers) do
      -- list[1] == method
      -- list[2] == target
      -- list[3] == target_idx
      -- set remote property
      -- target:method(target_idx, value)
      list[1](list[2], list[3], value)
    end
  end
end
lib.__newindex = set_property

local function add_observer(self, prop_name, target_method, target, target_idx)
  local prop = self.properties[prop_name]
  if not prop then
    error(string.format("Cannot connect '%s' to '%s' (source property does not exist)", prop_name, target_idx))
  end
  local observers = prop.observers
  -- make sure we do not connect twice
  for _, observer in ipairs(observers) do
    if observer[1] == target_method and observer[2] == target and observer[3] == target_idx then
      return
    end
  end
  -- add observer
  table.insert(observers, {target_method, target, target_idx})
end
lib.add_observer = add_observer

local function node_connect_observer(node, prop_name, arg2, arg3, arg4)
  if not arg4 then
    -- arg2 == node
    -- arg3 == target_idx
    add_observer(node.prop, prop_name, set_property, arg2.prop, arg3)
  else
    -- arg2 == target_method
    -- arg3 == target
    -- arg4 == target_idx
    add_observer(node.prop, prop_name, arg2, arg3, arg4)
  end
end

local function remove_observer(self, prop_name, target_method, target, target_idx)
  local prop = self.properties[prop_name]
  if not prop then
    -- ignore
    return
  end
  local observers = prop.observers

  -- remove connected observer
  local i = #observers
  while i > 0 do
    local observer = observers[i]
    if observer[1] == target_method and observer[2] == target and (target_idx == nil or observer[3] == target_idx) then
      -- remove element i from table
      table.remove(observers, i)
    end
    i = i - 1
  end
end
lib.remove_observer = remove_observer

local function node_disconnect_observer(node, arg1, arg2, arg3)
  if arg2 == nil then
    -- arg1 == target
    -- disconnect observer from all properties
    for prop_name, _ in pairs(node.prop.properties) do
      remove_observer(node.prop, prop_name, set_property, arg1.prop)
    end
  elseif arg3 == nil then
    -- arg1 = prop_name
    -- arg2 = target node
    -- disconnect observer from property 'prop_name'
    remove_observer(node.prop, arg1, set_property, arg2.prop)
  else
    -- arg1 = prop_name
    -- arg2 = target
    -- arg3 = target_idx
    -- disconnect observer hook 'target_idx' from property 'prop_name'
    remove_observer(node.prop, arg1, set_property, arg2.prop, arg3)
  end
end

setmetatable(lib, {
  -- new method
 __call = function(table, node)
  local instance = {properties = {}, node = node, values = {}}
  -- declare property methods
  -- FIXME: move these into lk.Node
  node.property   = node_declare_property
  node.connect    = node_connect_observer
  node.disconnect = node_disconnect_observer
  node.prop = instance
  setmetatable(instance, lib)
  return instance
end})


function lib:__index(idx)
  return rawget(self.values, idx)
end

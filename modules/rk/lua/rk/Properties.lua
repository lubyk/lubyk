--[[------------------------------------------------------

  rk.Properties
  -------------

  This enables state storage with notifications. The
  properties act like both inlets and outlets. As an inlet,
  the property can be given a callback triggering a private
  function. As an outlet, any external node can register
  a callback function.

--]]------------------------------------------------------
local lib = {}
rk.Properties = lib

setmetatable(lib, {
  -- new method
 __call = function(table)
  local instance = {prop = {}, observers = {}, callbacks = {}}
  setmetatable(instance, lib)
  return instance
end})

function lib.__newindex(table, idx, value)
  local old_value = table.prop[idx]
  local callback  = table.callbacks[idx]
  if callback then
    value = callback.method(callback.node, value)
  end
  table.prop[idx] = value
  if value ~= old_value then
    -- notify value change
    local observers = table.observers[idx]
    if observers then
      for obj, list in pairs(observers) do
        for method_name, _ in pairs(list) do
          local method = obj[method_name]
          if not method then
            -- remove ?
            -- list[method_name] = nil
          else
            method(obj, value)
          end
        end
      end
    end
  end
end

function lib.__index(table, idx)
  return rawget(lib, idx) or rawget(table.prop, idx)
end

function lib:connect(idx, obj, func_name)
  local observers = self.observers[idx]
  if not observers then
    observers = {}
    self.observers[idx] = observers
  end
  local obj_methods = observers[obj]
  if not obj_methods then
    obj_methods  = {}
    observers[obj] = obj_methods
  end

  obj_methods[func_name] = true
end


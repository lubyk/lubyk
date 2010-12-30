local lib = {}
Property = lib

setmetatable(lib, {
  -- new method
 __call = function(table)
  local instance = {prop = {}}
  setmetatable(instance, lib)
  return instance
end})

function lib.__newindex(table, idx, value)
  rawset(table.prop, idx, value)
  -- notify value change
  print(idx, value)
end

function lib.__index(table, idx)
  return rawget(table.prop, idx)
end

x = Property()

x.foo = 5
x.foo = 6
print(x.foo)
--[[------------------------------------------------------

  lk.Debug
  --------

  This is a helper to display debug information

--]]------------------------------------------------------

local lib = {}
lib.__index = lib
lk.Debug = lib

setmetatable(lib, {
  -- new method
 __call = function(table, arg)
  local instance = {path = arg}
  setmetatable(instance, lib)
  return instance
end})

function lib.inspect(table)
  for k,v in pairs(table) do
    print(k, v)
    if type(v) == 'table' then
      for k2, v2 in pairs(v) do
        print('', k2, v2)
      end
    end
  end
end
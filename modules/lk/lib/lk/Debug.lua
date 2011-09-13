--[[------------------------------------------------------

  lk.Debug
  --------

  This is a helper to display information on object
  creation and destruction. Pass any information when
  creating the object and these information will be
  printed out on destruction.

  Usage: lk.Debug('some name', ...)

--]]------------------------------------------------------
require 'lk.Debug_core'
local constr = lk.Debug
local lib   = {}
lib.__index = lib
lk.Debug = lib

setmetatable(lib, {
  -- new method
 __call = function(lib, ...)
  return constr(...)
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

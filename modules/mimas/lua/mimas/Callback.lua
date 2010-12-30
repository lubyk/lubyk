--[[------------------------------------------------------

  mimas.Callback
  --------------

  GUI event callback (can be connected to widget signals).

--]]------------------------------------------------------
require 'mimas.Callback_core'
require 'worker'
local worker = worker
local constr = mimas.Callback
local mt = nil
function mimas.Callback(func)
  local instance = constr(worker, func)
  if not mt then
    -- we have to wait for the first instance in order to
    -- access the metatable and rewrite the "connect" method.
    mt = getmetatable(instance)
    local connect = mt.connect
    function mt.connect(self, other, target)
      local callback = nil
      if string.match(target, '%(int%)') then
        callback = '1callback(int)'
      else
        callback = '1callback()'
      end
      if not string.match(target, '%(') then
        target = string.format("2%s()", target)
      else
        target = string.format("2%s", target)
      end
      connect(self, other:object(), target, callback)
    end
  end
  return instance
end


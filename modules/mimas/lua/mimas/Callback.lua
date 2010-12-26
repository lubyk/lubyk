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
    function mt.connect(self, other, meth)
      if not string.match(meth, '\\(') then
        meth = string.format("2%s()", meth)
      else
        meth = string.format("2%s", meth)
      end
      connect(self, other:object(), meth)
    end
  end
  return instance
end


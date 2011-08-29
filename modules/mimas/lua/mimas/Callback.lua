--[[------------------------------------------------------

  mimas.Callback
  --------------

  GUI event callback (can be connected to widget signals).

--]]------------------------------------------------------
require 'worker'
local worker = worker
local constr = mimas_core.Callback
local mt = mimas_core.Callback_
local connect = mt.connect

function mt.connect(self, other, target)
  local callback = nil
  if string.match(target, '%(double%)') then
    callback = '1callback(double)'
  elseif string.match(target, '%(int%)') then
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

function mt:callback()
  print('dummy callback')
end

function mimas.Callback(func)
  local self = constr()
  self.callback = func
  return self
end


--[[------------------------------------------------------

  wii.Remote
  ----------

  Lets you send and receive lua messages using 0MQ. Since
  a socket should only be used from a single thread, you
  can pass a function that will be executed in a new thread
  if you need to.

--]]------------------------------------------------------
require 'wii'
require 'wii.Browser_core'
require 'worker'

local constr = wii.Browser
local worker = worker
function wii.Browser(type, func)
  local instance = constr(worker, type)
  if func then
    instance.found = func
  end
  return instance
end

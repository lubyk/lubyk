--[[------------------------------------------------------

  wii.Remote
  ----------

  Lets you send and receive lua messages using 0MQ. Since
  a socket should only be used from a single thread, you
  can pass a function that will be executed in a new thread
  if you need to.

--]]------------------------------------------------------
require 'wii'
require 'wii.Remote_core'
require 'worker'

local constr = wii.Remote
local worker = worker
function wii.Remote(type, func)
  -- if func exists: do not create socket yet (wait for set_callback)
  local instance = constr(worker, type)
  if func then
    instance:set_callback(func)
  end
  return instance
end

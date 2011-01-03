--[[------------------------------------------------------

  zmq.Socket
  ----------

  Lets you send and receive lua messages using 0MQ. Since
  a socket should only be used from a single thread, you
  can pass a function that will be executed in a new thread
  if you need to.

--]]------------------------------------------------------
require 'zmq'
require 'rk' -- we use rk::Thread internally
require 'zmq.Socket_core'
require 'worker'

local constr = zmq.Socket
local worker = worker
function zmq.Socket(type, func)
  -- if func exists: do not create socket yet (wait for set_callback)
  local instance = constr(worker, type, not func)
  if func then
    instance:set_callback(func)
  end
  return instance
end

--[[------------------------------------------------------

  zmq.Receive
  -----------

  Lets you receive lua messages using 0MQ.

--]]------------------------------------------------------
require 'zmq'
require 'zmq.Socket'

function zmq.Pull(location, func)
  local instance = zmq.Socket(zmq.PULL)
  instance:connect(location)
  instance:loop(function()
    return func(instance:recv())
  end)
  return instance
end

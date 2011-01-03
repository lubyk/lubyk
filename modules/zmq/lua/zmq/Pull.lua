--[[------------------------------------------------------

  zmq.Receive
  -----------

  Lets you receive lua messages using 0MQ.

--]]------------------------------------------------------
require 'zmq'
require 'zmq.Socket'

function zmq.Pull(location, func)
  local instance
  instance = zmq.Socket(zmq.PULL, function()
    instance:connect(location)
    while instance:should_run() do
      -- receive, receive, ...
      func(instance:recv())
    end
  end)
  -- Sleep so that we let server start
  -- in order to have port and initialize the instance
  worker:sleep(10)

  return instance
end

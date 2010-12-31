--[[------------------------------------------------------

  zmq.Subscriber
  --------------

  Lets you receive lua messages using 0MQ.

--]]------------------------------------------------------
require 'zmq'
require 'zmq.Socket'

function zmq.Sub(func)
  local instance = zmq.Socket(zmq.SUB)
  instance:setsockopt(zmq.SUBSCRIBE) -- filter none
  instance:loop(function()
  print("loop")
  worker:sleep(100)
    --local msg = {instance:recv()}
    --print(unpack(msg))
    --return func(unpack(msg))
  end)
  return instance
end

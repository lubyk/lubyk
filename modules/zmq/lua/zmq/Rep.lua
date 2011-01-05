--[[------------------------------------------------------

  zmq.Rep
  -------

  Waits for messages and sends a reply back.

--]]------------------------------------------------------
require 'zmq'
require 'zmq.Socket'

function zmq.Rep(location, func)
  if not func then
    func = location
    location = nil
  end
  local instance
  instance = zmq.Socket(zmq.REP, function()
    if location then
      instance:bind(location)
    else
      -- choose a random port with "tcp://*"
      instance:bind_to_random_port()
    end

    while instance:should_run() do
      print("===", instance:should_run())
      -- receive, send, receive, ...
      instance:send(func(instance:recv()))
      print("===", instance:should_run())
    end
    print("===", instance:should_run())
  end)
  -- Sleep so that we let server start
  -- in order to have port and initialize the instance
  sleep(10)

  return instance
end

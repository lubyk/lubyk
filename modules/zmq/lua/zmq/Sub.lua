--[[------------------------------------------------------

  zmq.Subscriber
  --------------

  Lets you receive lua messages using 0MQ.

--]]------------------------------------------------------
require 'zmq'
require 'zmq.Socket'

function zmq.Sub(a, b, c)
  local instance, filter, func, setup_func

  if c then
    filter     = a
    setup_func = b
    func       = c
  elseif b then
    if type(a) == 'function' then
      filter     = nil
      setup_func = a
      func       = b
    else
      filter     = a
      setup_func = nil
      func       = b
    end
  else
    filter     = nil
    setup_func = nil
    func       = a
  end

  if func then
    instance = zmq.Socket(zmq.SUB, function()
      if filter then
        instance:setsockopt(zmq.SUBSCRIBE, filter)
      else
        instance:setsockopt(zmq.SUBSCRIBE) -- filter none = get all
      end

      if setup_func then
        setup_func(instance)
      end

      while instance:should_run() do
        -- receive, receive
        func(instance:recv())
      end
    end)
    -- Sleep so that we let server start
    sleep(10)
  else
    instance = zmq.Socket(zmq.SUB)
    if filter then
      instance:setsockopt(zmq.SUBSCRIBE, filter)
    else
      instance:setsockopt(zmq.SUBSCRIBE) -- filter none = get all
    end
  end
  return instance
end

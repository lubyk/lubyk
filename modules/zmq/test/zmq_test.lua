--[[------------------------------------------------------

  zmq for Lua
  -----------

  zmq with callbacks to integrate with rubyk.

--]]------------------------------------------------------
require 'rubyk'

-- zmq + mDNS = rk.Service
-- service = rk.Service("This is my name", "_rubyk._tcp", function(message)
--   print('Received', message)
-- end)

-- Url based slot:
-- service = rk.Service("This is my name", "_rubyk._tcp")

-- slot = service:slot("/some/url", function(message)
--   -- do something with message
--   return 343 ?
--   notify ?
-- end)

-- signal = rk.Signal(service) -- uses service for network connections
-- signal:connect("Other service", "/other/url")
-- signal:connect("/local/url")
-- signal:emit(4.5)

local should = test.Suite('zmq')

function should.send_and_receive()
  local sender   = zmq.Send("tcp://*:4455")
  local continue = false
  local receiver = zmq.Receive("tcp://localhost:4455", function(message)
    continue = true
  end)

  sender:send("Hello Rubyk!")
  while not continue do
    worker:sleep(10)
  end
end

test.all()

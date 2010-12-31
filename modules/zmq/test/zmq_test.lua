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

local port = 5000
function send_url()
  port = port + 1
  return string.format("tcp://*:%i", port)
end

function receive_url()
  return string.format("tcp://localhost:%i", port)
end

function should.send_and_receive()
  local sender   = zmq.Sender(send_url())
  local continue = false
  local received = nil
  local receiver = zmq.Receiver(receive_url(), function(...)
    continue = true
    received = arg
  end)

  local function send_and_receive(...)
    continue = false
    -- cannot use ... or arg is set to nil
    sender:send(unpack(arg))
    while not continue do
      worker:sleep(1)
    end
    for i, v in ipairs(arg) do
      assert_equal(arg[i], received[i])
    end
  end

  -- string
  send_and_receive("Hello Rubyk!")
  -- number, nil, bool
  send_and_receive(1.234567)
  send_and_receive(nil)
  send_and_receive(true)
  send_and_receive(false)
  -- array
  -- we cannot send an array like this
  -- send_and_receive({1, 2, 3}) ===> received as (1, 2, 3) (not {1, 2, 3})
  send_and_receive(1, {1, 2, 3})

  send_and_receive(1, {1, 2, {4, 5}})

  send_and_receive("1", {1, "zombie", {4.44, 5}})

  -- hash
  send_and_receive("/one/two", {one=2, three="four"})

  -- multi values
  send_and_receive(1,2,3)
  send_and_receive("/amp/gain", 3.5)
  send_and_receive("/amp/gain", {1, 2, {"foo", "bar", 5}})
end

function should.send_and_receive_many_messages()
  local sender   = zmq.Sender(send_url())
  local received = 0
  local receiver = zmq.Receiver(receive_url(), function(message)
    received = received + 1
  end)

  while received < 10 do
    sender:send("anything")
    worker:sleep(1)
  end

  assert_equal(10, received)
end

function should.publish_and_subscribe()
  local sender   = zmq.Publisher(send_url())
  local received = 0
  local receiver = zmq.Subscriber(function(message)
    received = received + 1
  end)

  receiver:connect(receive_url())

  while received < 10 do
    sender:send("anything")
    worker:sleep(1)
  end

  assert_equal(10, received)
end

function should.publish_and_subscribe_many()
  local sender   = zmq.Publisher(send_url())
  local received = 0
  local function receive_callback(message)
    received = received + 1
  end
  local receiver1 = zmq.Subscriber(receive_callback)
  local receiver2 = zmq.Subscriber(receive_callback)

  receiver1:connect(receive_url())
  receiver2:connect(receive_url())

  -- make sure receivers are ready before starting to send
  worker:sleep(10)

  sender:send("anything")

  while received < 2 do
    worker:sleep(10)
  end

  assert_equal(2, received)
end

test.all()
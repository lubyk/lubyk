--[[------------------------------------------------------

  zmq for Lua
  -----------

  zmq using msgpack to send/receive packets. The API is
  slightly different from ØMQ because of the callback
  nature of Lubyk bindings.

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('zmq.Pub/Sub')

function should.publishAndSubscribe()
  local sender   = zmq.Pub()
  local received = 0
  local receiver = zmq.Sub(function(server)
    while server:shouldRun() and server:recv() do
      received = received + 1
    end
  end)

  receiver:connect(string.format("tcp://localhost:%i", sender:port()))

  while received < 10 do
    sender:send("anything")
    sleep(10)
  end

  assertEqual(10, received)
  receiver:kill()
end

function should.shareRequestBetwenThreads(t)
  local thread_count = 5
  local up_count     = 3

  t.total = 0

  local sub = zmq.Sub(function(server)
    while server:shouldRun() do
      t.total = t.total + server:recv()
    end
  end)

  -- publisher
  local pub = zmq.Pub()
  sub:connect(string.format("tcp://localhost:%i", pub:port()))

  t.threads = {}
  for i=1,thread_count do
    t.threads[i] = lk.Thread(function()
      for j=1,up_count do
        sleep(10)
        pub:send(1)
      end
    end)
    -- adds 5 * 30 => 150
  end
  for i=1,thread_count do
    t.threads[i]:join()
  end

  while t.total < up_count * thread_count do
    sleep(10)
  end
  assertEqual(up_count * thread_count, t.total)
  sub:kill()
end

function should.publishAndSubscribeMany(t)
  local sender   = zmq.Pub()
  local received = 0

  local function receive_func(val)
    received = received + val
  end

  local receiver1 = zmq.SimpleSub(receive_func)
  local receiver2 = zmq.SimpleSub(receive_func)

  receiver1:connect(string.format("tcp://localhost:%i", sender:port()))
  receiver2:connect(string.format("tcp://localhost:%i", sender:port()))

  -- make sure receivers are ready before starting to send
  sleep(100)

  sender:send(1)

  while received < 2 do
    sleep(10)
  end

  assertEqual(2, received)
  receiver1:kill()
  receiver2:kill()
end

test.all()

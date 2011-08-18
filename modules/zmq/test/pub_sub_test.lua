--[[------------------------------------------------------

  zmq for Lua
  -----------

  zmq using msgpack to send/receive packets. The API is
  slightly different from ØMQ because of the callback
  nature of Lubyk bindings.

--]]------------------------------------------------------
require 'lubyk'


local should = test.Suite('zmq.Pub/Sub')

function should.publish_and_subscribe()
  local sender   = zmq.Pub()
  local received = 0
  local receiver = zmq.Sub(function(server)
    while server:should_run() and server:recv() do
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

function should.share_request_betwen_threads(t)
  local thread_count = 50
  local up_count     = 30

  t.total = 0

  t.sub = zmq.Sub(function(server)
    while server:should_run() do
      t.total = t.total + server:recv()
    end
  end)

  -- publisher
  t.pub = zmq.Pub()
  t.sub:connect(string.format("tcp://localhost:%i", t.pub:port()))

  t.threads = {}
  for i=1,thread_count do
    t.threads[i] = lk.Thread(function()
      for j=1,up_count do
        t.pub:send(1)
        sleep(10)
      end
    end)
    -- adds 5 * 30 => 150
  end
  for i=1,thread_count do
    t.threads[i]:join()
  end

  assertEqual(up_count * thread_count, t.total)
  t.sub:kill()
end

function should.publish_and_subscribe_many(t)
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

--[[------------------------------------------------------

  zmq for Lua
  -----------

  zmq using msgpack to send/receive packets. The API is
  slightly different from ØMQ because of the callback
  nature of Rubyk bindings.

--]]------------------------------------------------------
require 'rubyk'


local should = test.Suite('zmq')

local port = 5000
local function bind_url()
  port = port + 1
  return string.format("tcp://*:%i", port)
end

local function remote_url()
  return string.format("tcp://localhost:%i", port)
end

--function should.send_and_receive()
--  local sender   = zmq.Push(bind_url())
--  local continue = false
--  local received = nil
--  local receiver = zmq.Pull(remote_url(), function(...)
--    continue = true
--    received = arg
--  end)
--
--  local function send_and_receive(...)
--    continue = false
--    -- cannot use ... or arg is set to nil
--    print("Sending", unpack(arg))
--    sender:send(unpack(arg))
--    while not continue do
--      print('.')
--      worker:sleep(10)
--    end
--
--    for i, v in ipairs(arg) do
--      assert_equal(arg[i], received[i])
--    end
--  end
--
--  -- string
--  send_and_receive("Hello Rubyk!")
--  -- number, nil, bool
--  send_and_receive(1.234567)
--  send_and_receive(nil)
--  send_and_receive(true)
--  send_and_receive(false)
--  -- array
--  -- we cannot send an array like this
--  -- send_and_receive({1, 2, 3}) ===> received as (1, 2, 3) (not {1, 2, 3})
--  send_and_receive(1, {1, 2, 3})
--
--  send_and_receive(1, {1, 2, {4, 5}})
--
--  send_and_receive("1", {1, "zombie", {4.44, 5}})
--
--  -- hash
--  send_and_receive("/one/two", {one=2, three="four"})
--
--  -- multi values
--  send_and_receive(1,2,3)
--  send_and_receive("/amp/gain", 3.5)
--  send_and_receive("/amp/gain", {1, 2, {"foo", "bar", 5}})
--
--  receiver:kill()
--end

--function should.push_pull_many_messages()
--  local sender   = zmq.Push(bind_url())
--  local received = 0
--  local receiver = zmq.Pull(remote_url(), function(message)
--    received = received + 1
--  end)
--
--  while received < 10 do
--    sender:send("anything")
--    worker:sleep(1)
--  end
--
--  assert_equal(10, received)
--  receiver:kill()
--end
--
--function should.publish_and_subscribe()
--  local sender   = zmq.Pub(bind_url())
--  local received = 0
--  local receiver = zmq.Sub(function(message)
--    received = received + 1
--  end)
--
--  receiver:connect(remote_url())
--
--  while received < 10 do
--    sender:send("anything")
--    worker:sleep(1)
--  end
--
--  assert_equal(10, received)
--  receiver:kill()
--end

function should.publish_and_subscribe_many()
  local sender   = zmq.Pub(bind_url())
  local received = 0
  local function receive_callback(message)
    received = received + 1
  end
  local receiver1 = zmq.Sub(receive_callback)
  --local receiver2 = zmq.Sub(receive_callback)

  receiver1:connect(remote_url())
  --receiver2:connect(remote_url())

  -- make sure receivers are ready before starting to send
  worker:sleep(10)

  sender:send("anything")

  while received < 2 do
    worker:sleep(10)
  end

  assert_equal(2, received)
  receiver1:kill()
  --receiver2:kill()
end

--function should.request_reply()
--  local server = zmq.Rep(bind_url(), function(msg)
--    return msg + 4
--  end)
--
--  local total = 0
--  -- requester
--  local req = zmq.Req()
--
--  assert_equal(9, req:request(5))
--  server:kill()
--end

test.all()
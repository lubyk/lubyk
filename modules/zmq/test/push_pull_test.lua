--[[------------------------------------------------------

  zmq for Lua
  -----------

  zmq using msgpack to send/receive packets. The API is
  slightly different from ØMQ because of the callback
  nature of Lubyk bindings.

--]]------------------------------------------------------
require 'lubyk'


local should = test.Suite('zmq.Push/Pull')

function should.send_and_receive()
  local sender   = zmq.Push()
  local continue = false
  local received = nil
  local receiver = zmq.SimplePull(string.format("tcp://localhost:%i", sender:port()), function(...)
    continue = true
    received = arg
  end)

  local function send_and_receive(...)
    continue = false
    -- cannot use ... or arg is set to nil
    sender:send(unpack(arg))
    while not continue do
      sleep(10)
    end

    assertValueEqual(arg, received)
  end

  -- string
  send_and_receive("Hello Lubyk!")
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
  -- nested hash
  send_and_receive("/one/two", {one=2, three={'four', 5, 'six'}})

  -- multi values
  send_and_receive(1,2,3)
  send_and_receive("/amp/gain", 3.5)
  send_and_receive("/amp/gain", {1, 2, {"foo", "bar", 5}})

  receiver:kill()
end

function should.push_pull_many_messages()
  local sender   = zmq.Push()
  local received = 0
  local receiver = zmq.SimplePull(string.format("tcp://localhost:%i", sender:port()), function()
    received = received + 1
  end)

  receiver:connect(string.format("tcp://localhost:%i", sender:port()))

  for i=1,10 do
    sender:send("anything")
    sleep(10)
  end

  assertEqual(10, received)
  receiver:kill()
end

function should.enableCustomProperties()
  local sender = zmq.Push()
  sender.foo = 'bar'
  assertEqual('bar', sender.foo)
end


test.all()

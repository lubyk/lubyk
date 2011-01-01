--[[------------------------------------------------------

  zmq for Lua
  -----------

  zmq using msgpack to send/receive packets. The API is
  slightly different from ØMQ because of the callback
  nature of Rubyk bindings.

--]]------------------------------------------------------
require 'rubyk'


local should = test.Suite('zmq.Push / zmq.Pull')

function should.send_and_receive()
  local sender   = zmq.Push()
  local continue = false
  local received = nil
  local receiver = zmq.Pull(string.format("tcp://localhost:%i", sender:port()), function(...)
    continue = true
    received = arg
  end)

  local function send_and_receive(...)
    continue = false
    -- cannot use ... or arg is set to nil
    sender:send(unpack(arg))
    while not continue do
      worker:sleep(10)
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

  receiver:kill()
end

function should.push_pull_many_messages()
  local sender   = zmq.Push()
  local received = 0
  local receiver = zmq.Pull(string.format("tcp://localhost:%i", sender:port()), function(message)
    received = received + 1
  end)

  while received < 10 do
    sender:send("anything")
    worker:sleep(1)
  end

  assert_equal(10, received)
  receiver:kill()
end


test.all()
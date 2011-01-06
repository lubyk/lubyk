--[[------------------------------------------------------

  zmq for Lua
  -----------

  zmq using msgpack to send/receive packets. The API is
  slightly different from ØMQ because of the callback
  nature of Rubyk bindings.

--]]------------------------------------------------------
require 'rubyk'


local should = test.Suite('zmq.Push/Pull')

--function should.send_and_receive()
--  local sender   = zmq.Push()
--  local continue = false
--  local received = nil
--  local receiver = zmq.SimplePull(string.format("tcp://localhost:%i", sender:port()), function(...)
--    continue = true
--    received = arg
--  end)
--
--  local function send_and_receive(...)
--    continue = false
--    -- cannot use ... or arg is set to nil
--    sender:send(unpack(arg))
--    while not continue do
--      sleep(10)
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

function should.push_pull_many_messages(t)
  t.sender   = zmq.Push()
  print(t.sender)
  t.received = 0
  t.receiver = zmq.SimplePull(string.format("tcp://localhost:%i", t.sender:port()), function()
    received = received + 1
  end)

  for i=1,10 do
    t.sender:send("anything")
    print('done')
    sleep(1)
  end

  assert_equal(10, t.received)
  t.receiver:kill()
end


test.all()
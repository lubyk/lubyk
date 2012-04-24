--[[------------------------------------------------------

  zmq for Lua
  -----------

  zmq using msgpack to send/receive packets. The API is
  slightly different from ØMQ because of the callback
  nature of Lubyk bindings.

--]]------------------------------------------------------
require 'lubyk'


local should = test.Suite('zmq.Req/Rep')

function should.requestAndReply()
  -- reply
  local receiver = zmq.SimpleRep(function(...)
    return ...
  end)

  -- requester
  local req = zmq.Req()
  req:connect(string.format("tcp://localhost:%i", receiver:port()))


  local function sendAndReceive(...)
    local arg = {...}
    assertValueEqual(arg, {req:request(...)})
  end

  -- string
  sendAndReceive("Hello Lubyk!")
  -- number, nil, bool
  sendAndReceive(1.234567)
  sendAndReceive(nil)
  sendAndReceive(true)
  sendAndReceive(false)
  -- array
  -- we cannot send an array like this
  -- sendAndReceive({1, 2, 3}) ===> received as (1, 2, 3) (not {1, 2, 3})
  sendAndReceive(1, {1, 2, 3})

  sendAndReceive(1, {1, 2, {4, 5}})

  sendAndReceive("1", {1, "zombie", {4.44, 5}})

  -- hash
  sendAndReceive("/one/two", {one=2, three="four"})
  -- nested hash
  sendAndReceive("/one/two", {one=2, three={'four', 5, 'six'}})

  -- multi values
  sendAndReceive(1,2,3)
  sendAndReceive("/amp/gain", 3.5)
  sendAndReceive("/amp/gain", {{x = "foo"}, {bar = 5}})

  receiver:kill()
end
--function should.share_request_betwen_threads(t)
--  local thread_count = 100
--  local up_count     = 30
--  t.server = zmq.SimpleRep(function(a, b)
--    return a + b
--  end)
--
--  t.total = 0
--  -- requester
--  t.req = zmq.Req()
--  t.req:connect(string.format("tcp://localhost:%i", t.server:port()))
--
--  t.threads = {}
--  for i=1,thread_count do
--    t.threads[i] = lk.Thread(function()
--      for j=1,up_count do
--        local val = t.req:request(i, j)
--        --print(i, j, val)
--        if val == i + j then
--          t.total = t.total + 1
--        end
--        -- adds 2, 4, 6, 8, 10 ==> 30
--        sleep(1)
--      end
--    end)
--    -- adds 5 * 30 => 150
--  end
--  for i=1,thread_count do
--    t.threads[i]:join()
--  end
--
--  assertEqual(up_count * thread_count, t.total)
--  t.server:kill()
--end

test.all()

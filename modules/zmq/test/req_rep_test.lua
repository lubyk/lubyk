--[[------------------------------------------------------

  zmq for Lua
  -----------

  zmq using msgpack to send/receive packets. The API is
  slightly different from ØMQ because of the callback
  nature of Lubyk bindings.

--]]------------------------------------------------------
require 'lubyk'


local should = test.Suite('zmq.Req/Rep')

function should.request_reply()
  local server = zmq.SimpleRep(function(msg)
    return msg + 4
  end)

  local total = 0
  -- requester
  local req = zmq.Req()
  req:connect(string.format("tcp://localhost:%i", server:port()))
  assert_equal(9, req:request(5))
  server:kill()
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
--    t.threads[i] = rk.Thread(function()
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
--  assert_equal(up_count * thread_count, t.total)
--  t.server:kill()
--end

test.all()
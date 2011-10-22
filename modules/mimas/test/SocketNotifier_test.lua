--[[------------------------------------------------------

  mimas.SocketNotifier test
  -------------------------

  We also test zmq.Socket REQ/REP with the
  mimas poller which uses the SocketNotifier internally.

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('mimas.SocketNotifier')

function should.autoload()
  assertTrue(mimas.SocketNotifier)
end

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

function should_enableFdInSelect()
  -- reply
  local receiver = zmq.SimpleRep(function(...)
    print('REP received', ...)
    return ...
  end)

  -- requester
  local req = zmq.Req()
  req:connect(string.format("tcp://localhost:%i", receiver:port()))

  local function sendAndReceive(...)
    assertValueEqual({req:request(unpack(arg))}, arg)
  end

  -- string
  sendAndReceive("Hello Lubyk!")
  -- number, nil, bool
  sendAndReceive(1.234567)
  receiver:kill()
end

test.all()

--[[------------------------------------------------------

  lk.Service
  -----------

  zmq + mDNS

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.Service')

function should.garbageCollectService()
  local deleted = false
  local service = lk.Service('ToBeDeleted')
  service.del = lk.Finalizer(function()
    deleted = true
  end)
  service = nil
  collectgarbage('collect')
  collectgarbage('collect')
  assertTrue(deleted)
end

function should.connectWhenRemoteAppears(t)
  local received_count = 0
  local function receiveMessage(message)
    received_count = received_count + 1
    if message == 'message from Mars' then
      t.continue = true
    end
  end

  -- create a client that checks for 'message from Mars'
  local venus = lk.Client(receiveMessage)

  -- subscribe to messages from 'Mars'
  -- connection goes from subscriber to publisher and can be initiated before
  -- Mars is even created.
  venus:subscribe('base:Mars')

  -- create a service called 'Mars' with default service type 'Lubyk.Tcp'
  -- and that replies to all messages with nil.
  local mars = lk.Service('base:Mars')

  -- connected becomes true when 'Mars' appears on the network
  t:timeout(function()
    return venus:connected()
  end)
  assertTrue(venus:connected())

  sleep(0.02) -- let zmq make connection before sending
  mars:notify('One')
  mars:notify('message from Mars')

  t:timeout(function()
    return t.continue
  end)

  assertEqual(2, received_count)
end

test.all()

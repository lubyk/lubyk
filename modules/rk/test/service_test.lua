--[[------------------------------------------------------

  rk.Service
  -----------

  zmq + mDNS

--]]------------------------------------------------------
require 'rubyk'

local should = test.Suite('rk.Service')

function should.connect_when_remote_appears()
  local continue = false
  local received_count = 0
  local function receive_message(self, message)
    received_count = received_count + 1
    if self.name == 'Venus' and message == 'message from Mars' then
      continue = true
    end
  end

  -- create a first service called 'Venus' that checks for 'message from Mars'
  local venus = rk.Service("Venus", receive_message)

  -- 'Venus' listens to messages from 'Mars'
  -- connection goes from subscriber to publisher and can be initiated before
  -- Mars is even created.
  venus:connect("Mars")

  -- create a second service called 'Mars' with default service type '_rubyk._tcp'
  -- and that does nothing when receiving messages.
  local mars = rk.Service("Mars")

  -- connected becomes true when 'Mars' appears on the network
  while not venus:connected() do
    worker:sleep(10) -- make sure everything is ready before sending
  end

  mars:send("One")
  mars:send("message from Mars")

  while not continue do
    worker:sleep(5)
  end

  assert_equal(2, received_count)
end

test.all()
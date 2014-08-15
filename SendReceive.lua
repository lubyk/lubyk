--[[------------------------------------------------------

  # Automatic script reloading (live coding)

  In this tutorial, we show how to use osc.Server and osc.Client to send and
  receive osc messages.
  
  We use lens.FileWatch to enable live coding for this script.

  ## Download source

  [SendReceive.lua](example/osc/SendReceive.lua)

--]]------------------------------------------------------
-- doc:lit

-- # Preamble
--
-- Require lens and osc libraries.
local lens = require 'lens'
local osc  = require 'osc'

-- Start scheduler and setup script reload hook with `lens.FileWatch`. Starting
-- the scheduler at the top of the script and using file reloading is a nice
-- trick that ensures all the code after `lens.run` is only executed within the
-- scope of the scheduler.
lens.run(function() lens.FileWatch() end)

-- Create a server to listen to incoming OSC messages on port `11000`.
server = server or osc.Server(11000)

-- Setup our callback on received messages.
function server:receive(url, ...)
  print(url, ...)
end

-- Create a client connected to '127.0.0.1:11000'.
client = client or osc.Client('127.0.0.1', 11000)

-- Create a timer with 0.2s interval.
timer = timer or lens.Timer(0.5)

-- Setup timer callback to send random values with our client.
function timer:timeout()
  client:send('/1/fader1', math.random())
end
timer:start()

--[[
  ## Download source

  [SendReceive.lua](example/osc/SendReceive.lua)
--]]


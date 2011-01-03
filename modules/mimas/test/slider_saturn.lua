--[[------------------------------------------------------

  Run this file after starting test_slider.lua
  --------------------------------------------

  zmq + mDNS

--]]------------------------------------------------------
require 'rubyk'

local continue = false
local value = 0

local function set_value(message)
  if message == 1.0 then
    continue = true
  end
  value = message
  -- value changed, notify
  print("Saturn --->", value)
  saturn:notify(value)
end

-- create a service called 'Saturn' that checks for messages from Mimas
saturn = rk.Service('Saturn', function(message)
  print("Saturn <---", message)
  set_value(message)
end)

local i = 0
-- random update of value
local timer = rk.Timer(20, function()
  i = i + 1
  set_value(0.5 + 0.49 * math.sin(i * math.pi / 20))
end)
timer:start()

while not continue do
  worker:sleep(20)
end

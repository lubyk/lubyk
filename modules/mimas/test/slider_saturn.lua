--[[------------------------------------------------------

  Run this file after starting test_slider.lua
  --------------------------------------------

  zmq + mDNS

--]]------------------------------------------------------
require 'rubyk'

continue = false
value = 0

function set_value(message)
  if message == 1.0 then
    continue = true
  end
  value = message
  -- value changed, notify
  print("Saturn --->", value)
  saturn:notify(value)
end

-- create a service called 'Saturn' with a Reply socket
saturn = rk.Service('Saturn', function(message)
  print("Saturn <---", message)
  set_value(message)
  if message == 1 then
    saturn:quit()
  end
  return message
end)

i = 0
-- random update of value
timer = rk.Timer(20, function()
  i = i + 1
  set_value(0.5 + 0.49 * math.sin(i * math.pi / 20))
end)
timer:start()

saturn:join()
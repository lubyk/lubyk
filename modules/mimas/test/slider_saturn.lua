--[[------------------------------------------------------

  Run this file after starting test_slider.lua
  --------------------------------------------

  zmq + mDNS

--]]------------------------------------------------------
require 'rubyk'

continue = false
value = 0

counter = 0
now = worker:now()
function set_value(message)
  counter = counter + 1
  if counter == 20 then
    if worker:now() - now > 0 then
      print('', '', 'packets/s', 20*1000 / (worker:now() - now))
    end
    counter = 0
    now = worker:now()
  end

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
  if message == nil then
    print('Quit')
    saturn.rep:kill()
    saturn.pull:quit()
  else
    set_value(message)
  end
  return message
end)

i = 0
-- random update of value
timer = rk.Timer(50, function()
  i = i + 1
  set_value(0.5 + 0.49 * math.sin(i * math.pi / 20))
end)
timer:start()

saturn:join()
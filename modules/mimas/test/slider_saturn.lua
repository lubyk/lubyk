--[[------------------------------------------------------

  Run this file along with slider_mimas.lua
  --------------------------------------------

  zmq + mDNS

--]]------------------------------------------------------
require 'lubyk'

continue = false
value = 0
counter = 0
now = worker:now()
function setValue(message)
  counter = counter + 1
  if counter == 20 then
    if worker:now() - now > 0 then
      --print('', '', 'packets/s', 20*1000 / (worker:now() - now))
    end
    counter = 0
    now = worker:now()
  end

  if message == 1.0 then
    continue = true
  end
  value = message
  -- value changed, notify
  saturn:notify(value)
end

-- create a service called 'Saturn' with a Reply socket
saturn = lk.Service('Saturn', function(message)
  if message == 'Quit' then
    print('Quit')
    sched:quit()
  else
    setValue(message)
  end
  return message
end)

i = 0
-- random update of value
timer = lk.Timer(100, function()
  i = i + 1
  setValue(0.5 + 0.49 * math.sin(i * math.pi / 20))
end)
timer:start()
print('Random updates started. Ready for GUI.')
run()

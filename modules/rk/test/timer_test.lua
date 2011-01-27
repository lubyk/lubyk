--[[------------------------------------------------------

  rk.Timer
  --------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('rk.Timer')
local counter = 0

function should.loop_timer_in_external_thread()
  local timer = rk.Timer(10, function()
    counter = counter + 1
    -- continue until 'timer' is gc or stopped.
  end)
  timer:start()
  -- first trigger now
  sleep(91)
  assert_equal(10, counter)
  sleep(100)
  assert_equal(20, counter)
  counter = 0
  timer:stop()
end

function should.join_timer()
  local timer = rk.Timer(10, function()
    counter = counter + 1
    if counter == 5 then
      -- stop
      return 0
    end
  end)
  timer:start()
  -- wait for timer to finish
  timer:join()
  assert_equal(5, counter)
end

test.all()
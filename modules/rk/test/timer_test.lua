--[[------------------------------------------------------

  rk.Timer test
  --------

  ...

--]]------------------------------------------------------
require 'rubyk'

local should = test.Suite('rk.Timer')
local counter = 0

function should.loop_timer_in_external_thread()
  local timer = rk.Timer(10, function()
    counter = counter + 1
    -- continue until 'timer' is gc or stopped.
  end)
  timer:start()
  -- first trigger now
  rubyk.sleep(91)
  assert_equal(10, counter)
  rubyk.sleep(100)
  assert_equal(20, counter)
end

test.all()
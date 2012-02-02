--[[------------------------------------------------------

  lk.Timer test
  -------------

  The timer calls the 'tick' method at regular intervals.

--]]------------------------------------------------------
require 'lubyk'

local SLEEP = 40
local should = test.Suite('lk.Timer')

function should.loopTimerInExternalThread()
  local counter = 0
  local timer = lk.Timer(SLEEP, function()
    counter = counter + 1
    -- continue until 'timer' is gc or stopped.
  end)
  timer:start() -- default = trigger on start
  -- first trigger now
  now = worker:now()
  sleep(4 * SLEEP + 10)
  -- 00, 20, 40, 60, 80
  assertEqual(5, counter)
  now = worker:now()
  sleep(5 * SLEEP)
  --                   , 100, 120, 140, 160, 180
  assertEqual(10, counter)
  timer:stop()
end

function should.setInterval()
  local counter = 0
  local timer = lk.Timer(10000, function()
    counter = counter + 1
    -- continue until 'timer' is gc or stopped.
  end)
  timer:start(false) -- do not trigger on start
  sleep(SLEEP - 10) -- 10
  assertEqual(0, counter)
  timer:setInterval(SLEEP)
  assertEqual(0, counter)
  sleep(4 * SLEEP) -- triggers:
  -- 20, 40, 60, 80
  assertEqual(4, counter)
  sleep(5 * SLEEP)
  assertEqual(9, counter)
  counter = 0
  timer:stop()
end

function should.setCallback()
  local counter = 0
  local timer = lk.Timer(10000)
  function timer:tick()
    counter = counter + 1
    -- continue until 'timer' is gc or stopped.
  end
  timer:start(false) -- do not trigger on start
  sleep(SLEEP - 10)
  assertEqual(0, counter)
  timer:setInterval(SLEEP)
  assertEqual(0, counter)
  sleep(4 * SLEEP) -- triggers:
  -- 20, 40, 60, 80
  assertEqual(4, counter)
  sleep(5 * SLEEP)
  assertEqual(9, counter)
  counter = 0
  timer:stop()
end

function should.joinTimer()
  local counter = 0
  local timer = lk.Timer(10, function()
    counter = counter + 1
    if counter == 5 then
      -- stop
      return 0
    end
  end)
  timer:start()
  -- wait for timer to finish
  timer:join()
  assertEqual(5, counter)
end

test.all()

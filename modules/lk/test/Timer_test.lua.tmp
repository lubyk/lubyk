--[[------------------------------------------------------

  lk.Timer
  --------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.Timer')

function should.loopTimerInExternalThread()
  local counter = 0
  local timer = lk.Timer(10, function()
    counter = counter + 1
    -- continue until 'timer' is gc or stopped.
  end)
  timer:start() -- default = trigger on start
  -- first trigger now
  sleep(91)
  assertEqual(10, counter)
  sleep(100)
  assertEqual(20, counter)
  counter = 0
  timer:stop()
end

function should.setInterval()
  local counter = 0
  local timer = lk.Timer(10000, function()
    counter = counter + 1
    -- continue until 'timer' is gc or stopped.
  end)
  timer:start(false) -- do not trigger on start
  sleep(10)
  assertEqual(0, counter)
  timer:setInterval(10)
  assertEqual(0, counter)
  sleep(91) -- triggers:
  -- 10, 20, 30, 40, 50, 60, 70, 80, 90
  assertEqual(10, counter)
  sleep(100)
  assertEqual(20, counter)
  counter = 0
  timer:stop()
end

function should.setCallback()
  local counter = 0
  local timer = lk.Timer(10000)
  function timer.tick()
    counter = counter + 1
    -- continue until 'timer' is gc or stopped.
  end
  timer:start(false) -- do not trigger on start
  sleep(10)
  assertEqual(0, counter)
  timer:setInterval(10)
  assertEqual(0, counter)
  sleep(91) -- triggers:
  -- 10, 20, 30, 40, 50, 60, 70, 80, 90
  assertEqual(10, counter)
  sleep(100)
  assertEqual(20, counter)
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
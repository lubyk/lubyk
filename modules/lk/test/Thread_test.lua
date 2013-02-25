--[[------------------------------------------------------

  lk.Thread
  ---------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.Thread')

function should.runThread()
  local counter = 0
  local start = sched.now
  local thread = lk.Thread(function()
    while true do
      counter = counter + 1
      -- continue until 'timer' is gc or stopped.
      sleep(0.04)
    end
  end)

  -- first trigger now
  sleep(0.1)
  -- 00, 40, 80
  assertEqual(3, counter)
  sleep(0.09)
  -- 120, 160 (190)
  assertEqual(5, counter)
  -- Thread is garbage collected here.
  -- This means that the scheduler does not hold on threads.
end

function should.joinThreads()
  local counter = 0
  local thread = lk.Thread(function()
    while counter < 5 do
      sleep(0.01)
      counter = counter + 1
    end
  end)

  -- wait for thread to finish
  thread:join()
  assertEqual(5, counter)
end

function should.killThreads()
  local thread = lk.Thread(function()
    while true do
      sleep(0.03)
    end
  end)
  sleep(0.02) -- let thread start

  thread:kill()

  thread:join()
  -- should not hang
  assertTrue(true)
end

local function makeThreads()
  collectgarbage('collect')
  local threads = {}
  for i = 1,200 do
    table.insert(threads, lk.Thread(function()
      -- do nothing
      local j = 0
      while j < 10 do
        j = j + 1
        sleep(0.01)
      end
    end))
  end
  
  for i, thread in ipairs(threads) do
    thread:join()
  end
end

function should.createManyThreadsAndProperlyGc()
  local before, after
  -- warmup
  makeThreads()
  collectgarbage('collect')
  collectgarbage('collect')
  before = collectgarbage('count')
  -- create
  makeThreads()
  -- destroy
  collectgarbage('collect')
  collectgarbage('collect')
  after = collectgarbage('count')

  print(before, after)
  assertTrue(after <= before)
end

function should.getKilledBeforeStarting()
  local runOnce = false
  local thread = lk.Thread(function(runner)
    runOnce = true
  end)

  -- stop
  thread:kill()

  assertFalse(runOnce)
end

function should.getDestroyedBeforeStarting()
  local runOnce = false
  local thread = lk.Thread(function()
    runOnce = true
  end)

  -- destroy
  thread = nil
  collectgarbage('collect')

  assertFalse(runOnce)
end

function should.getKilledOnDestroy()
  local runOnce = false
  local thread = lk.Thread(function(runner)
    runOnce = true
    while true do
      sleep(0.01)
    end
  end)
  -- start thread
  sleep(0.01)
  -- destroy
  thread = nil
  collectgarbage('collect')

  assertTrue(runOnce)
end

test.all()

--[[------------------------------------------------------

  rk.Thread
  ---------

  ...

--]]------------------------------------------------------
require 'rubyk'

local should = test.Suite('rk.Thread')

function should.run_thread()
  local counter = 0
  local thread = rk.Thread(function()
    while true do
      counter = counter + 1
      -- continue until 'timer' is gc or stopped.
      worker:sleep(10)
    end
  end)

  -- first trigger now
  worker:sleep(98)
  assert_equal(10, counter)
  worker:sleep(100)
  assert_equal(20, counter)
end

function should.join_threads()
  local counter = 0
  local thread = rk.Thread(function()
    while counter < 5 do
      worker:sleep(10)
      counter = counter + 1
    end
  end)

  -- wait for thread to finish
  thread:join()
  assert_equal(5, counter)
end

function should.kill_threads()
  local thread = nil
  thread = rk.Thread(function()
    while thread:should_run() do
      worker:sleep(30)
    end
  end)
  worker:sleep(20) -- let thread start

  thread:kill()

  thread:join()
  -- should not hang
  assert_true(true)
end

function should.not_segfault()
  local thread = nil
  for i = 1,1000 do
    rk.Thread(function()
      -- do nothing
    end)
    worker:sleep(1)
  end
  -- should not hang
  assert_true(true)
end

test.all()
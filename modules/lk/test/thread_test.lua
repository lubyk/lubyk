--[[------------------------------------------------------

  lk.Thread
  ---------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.Thread')

function should.run_thread()
  local counter = 0
  local thread = lk.Thread(function()
    while true do
      counter = counter + 1
      -- continue until 'timer' is gc or stopped.
      sleep(10)
    end
  end)

  -- first trigger now
  sleep(98)
  assert_equal(10, counter)
  sleep(100)
  assert_equal(20, counter)
end

function should.join_threads()
  local counter = 0
  local thread = lk.Thread(function()
    while counter < 5 do
      sleep(10)
      counter = counter + 1
    end
  end)

  -- wait for thread to finish
  thread:join()
  assert_equal(5, counter)
end

function should.kill_threads()
  local thread = nil
  thread = lk.Thread(function()
    while thread:should_run() do
      sleep(30)
    end
  end)
  sleep(20) -- let thread start

  thread:kill()

  thread:join()
  -- should not hang
  assert_true(true)
end

local function make_threads()

  collectgarbage('collect')
  local threads = {}
  for i = 1,200 do
    table.insert(threads, lk.Thread(function()
      -- do nothing
      local j = 0
      while j < 10 do
        j = j + 1
        sleep(20)
      end
    end))
  end

  for _, thread in ipairs(threads) do
    thread:join()
  end
end

function should.create_many_threads_and_properly_gc()
  -- warmup
  make_threads()

  collectgarbage('collect')
  collectgarbage('collect')
  local before = collectgarbage('count')
  make_threads()

  collectgarbage('collect')
  collectgarbage('collect')
  local after = collectgarbage('count')
  if #test.suites > 1 then
    -- with other tests messing around, we have to be more tolerant
    assert_less_then(before * 1.01, after)
  else
    -- when running the test alone
    assert_equal(before, after)
  end
end

function should.get_killed_before_starting()
  local run_once = false
  local thread = lk.Thread(function(runner)
    run_once = true
  end)

  -- stop
  thread:kill()

  assert_false(run_once)
end

function should.get_destroyed_before_starting()
  local run_once = false
  local thread = lk.Thread(function(runner)
    run_once = true
    print(runner)
  end)

  -- destroy
  thread = nil
  collectgarbage('collect')

  assert_false(run_once)
end

function should.get_killed_on_destroy()
  local run_once = false
  local thread = lk.Thread(function(runner)
    run_once = true
    while true do
      sleep(10)
    end
  end)
  -- start thread
  sleep(10)
  -- destroy
  thread = nil
  collectgarbage('collect')

  assert_true(run_once)
end

test.all()
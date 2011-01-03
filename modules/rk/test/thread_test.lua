--[[------------------------------------------------------

  rk.Thread
  ---------

  ...

--]]------------------------------------------------------
require 'rubyk'

local should = test.Suite('rk.Thread')

--function should.run_thread()
--  local counter = 0
--  local thread = rk.Thread(function()
--    while true do
--      counter = counter + 1
--      -- continue until 'timer' is gc or stopped.
--      worker:sleep(10)
--    end
--  end)
--
--  -- first trigger now
--  worker:sleep(98)
--  assert_equal(10, counter)
--  worker:sleep(100)
--  assert_equal(20, counter)
--end
--
--function should.join_threads()
--  local counter = 0
--  local thread = rk.Thread(function()
--    while counter < 5 do
--      worker:sleep(10)
--      counter = counter + 1
--    end
--  end)
--
--  -- wait for thread to finish
--  thread:join()
--  assert_equal(5, counter)
--end
--
--function should.kill_threads()
--  local thread = nil
--  thread = rk.Thread(function()
--    while thread:should_run() do
--      worker:sleep(30)
--    end
--  end)
--  worker:sleep(20) -- let thread start
--
--  thread:kill()
--
--  thread:join()
--  -- should not hang
--  assert_true(true)
--end

function should.not_segfault()
  local threads = {}
  local functions = {}
  collectgarbage('stop')
  for i = 1,100 do
    table.insert(threads, rk.Thread(function()
      -- do nothing
      local j = 0
      while j < 10 do
        print(i, j)
        j = j + 1
        worker:sleep(20)
      end
    end))
    if i == 60 then
      collectgarbage('collect')
      print("collect ok.")
    end
  end

  for _, thread in ipairs(threads) do
    thread:join()
  end

  -- should not hang
  assert_true(true)
end

test.all()
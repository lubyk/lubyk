--[[------------------------------------------------------

  worker_test
  -----------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should  = test.Suite('worker')

-- worker sleep
-- once the worker is started, we should be inside a coroutine
function should.sleep_in_main()
  local now = worker:now() -- time since process started
  sleep(20)
  assertTrue(worker:now() >= now + 20)
  assertTrue(worker:now() <= now + 21)
end

-- worker wait
-- once the worker is started, we should be inside a coroutine
function should.wait_in_main()
  local now = worker:now() -- time since process started
  wait(20)
  assertTrue(worker:now() >= now + 20)
  assertTrue(worker:now() <= now + 21)
end

test.all()
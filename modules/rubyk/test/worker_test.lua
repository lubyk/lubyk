--[[------------------------------------------------------

  worker_test
  -----------

  ...

--]]------------------------------------------------------
require 'rubyk'

local should  = test.Suite('worker')

-- worker sleep
-- once the worker is started, we should be inside a coroutine
function should.sleep_in_main()
  local now = worker:now() -- time since process started
  worker:sleep(200)
  assert(worker:now() >= now + 200)
  assert(worker:now() <= now + 201)
end

test.all()
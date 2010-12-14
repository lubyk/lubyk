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
  local now = rubyk.now() -- time since process started
  rubyk.sleep(200)
  assert(rubyk.now() >= now + 200)
  assert(rubyk.now() <= now + 202)
end

-- function should.spawn_external_thread()
--   local timer = rubyk.timer(10, function()
--     counter = counter + 1
--     -- continue until 'timer' is gc or stopped.
--   end)
--   rubyk.sleep(100)
--   print(counter)
--   rubyk.sleep(100)
--   print(counter)
-- end

test.all()



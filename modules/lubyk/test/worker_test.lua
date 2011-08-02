--[[------------------------------------------------------

  worker_test
  -----------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should  = test.Suite('worker')

-- worker sleep
-- once the worker is started, we should be inside a coroutine
function should.sleepInMain()
  local now = worker:now() -- time since process started
  sleep(20)
  assertTrue(worker:now() >= now + 20)
  assertTrue(worker:now() <= now + 21)
end

-- worker wait
-- once the worker is started, we should be inside a coroutine
function should.waitInMain()
  local now = worker:now() -- time since process started
  wait(20)
  assertTrue(worker:now() >= now + 20)
  assertTrue(worker:now() <= now + 21)
end

function should.returnExecPath()
  assertMatch('bin/lua', worker:execPath())
end

function should.spawnProcess()
  io.write('concurrent: ')
  io.flush()
  -- concurrent execution of the two loops
  local pid = worker:spawn[[
    require 'lubyk'
    for i=1,6 do
      sleep(20)
      io.write('.')
      io.flush()
    end
    worker:exit(5)
  ]]

  for i=1,5 do
    sleep(20)
    io.write('x')
    io.flush()
  end
  -- wait for pid to finish
  assertEqual(5, worker:waitpid(pid))
  print('')
end
test.all()
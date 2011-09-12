--[[------------------------------------------------------

  lk.Scheduler
  ------------

  Manages a main loop or passes this loop to the GUI if
  one is running.

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.Scheduler')
local sched_bak

function should.setup()
  sched_bak = sched
end

function should.teardown()
  sched = sched_bak
end

function should.autoload()
  assertType('table', lk.Scheduler)
end

function should.respondToThread(t)
  local sched = lk.Scheduler()
  local function foo() 
    t.continue = true
  end
  assertPass(function()
    sched:thread(foo)
  end)
  assertType('table', sched.at_next)
  coroutine.resume(sched.at_next.co)
  assertTrue(t.continue)
end

function should.run(t)
  local sched = lk.Scheduler()
  sched:thread(function()
    t.continue = true
  end)
  sched:run()
  assertTrue(t.continue)
end

function should.runWithSocket(t)
  sched = lk.Scheduler()
  local srv = lk.Socket(function(self)
    --print('[2] accept')
    local rem  = self:accept()
    --print('[7] remote = ', rem.sock_fd)
    --print('[8] srv recvLine')
    local data = rem:recvLine()
    --print('[9] srv received')
    assertEqual('Hello Lubyk!', data)
    t.continue = true
  end)
  --print('[1] bind & listen')
  local port = srv:bind('127.0.0.1', 0)
  srv:listen()

  -- client thread
  sched:thread(function()
    --print('[3] create cli')
    local cli = lk.Socket()
    --print('[4] cli connect')
    cli:connect('127.0.0.1', port)
    --print('[5] cli send')
    cli:send('Hello Lubyk!\n')
    --print('[6] cli done')
  end)
  
  -- runs until socket thread ends
  sched:run()
  assertTrue(t.continue)
end

test.all()

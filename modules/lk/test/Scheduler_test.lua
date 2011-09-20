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
  lk.Thread.setScheduler(sched)
end

function should.autoload()
  assertType('table', lk.Scheduler)
end

function should.createThread(t)
  local sched = lk.Scheduler()
  lk.Thread.setScheduler(sched)
  local function foo() 
    t.continue = true
  end
  local thread = lk.Thread(foo)
  assertEqual(thread.wrap, sched.at_next)
  assertEqual(0, thread.wrap.at)
  coroutine.resume(thread.co)
  assertTrue(t.continue)
end

function should.run(t)
  local sched = lk.Scheduler()
  lk.Thread.setScheduler(sched)
  local x = lk.Thread(function()
    t.continue = true
  end)
  sched:run()
  assertTrue(t.continue)
end

function should.runWithSocket(t)
  local sched = lk.Scheduler()
  lk.Thread.setScheduler(sched)
  local port
  local srv = lk.Socket(function(self)
    --print('[1] bind & listen')
    port = self:bind('127.0.0.1', 0)
    self:listen()
    --print('[2] accept')
    self:accept(function(cli)
      --print('[7] remote = ', cli.sock_fd)
      --print('[8] srv recvLine')
      local data = cli:recvLine()
      --print('[9] srv received')
      --print(string.format("'%s'", data))
      assertEqual('Hello Lubyk!', data)
      t.continue = true
    end)
  end)

  -- client thread
  local cli = lk.Socket(function(self)
    --print('[3] create cli')
    --print('[4] cli connect')
    self:connect('127.0.0.1', port)
    --print('[5] cli send')
    self:send('Hello Lubyk!\n')
    --print('[6] cli done')
  end)
  
  -- runs until socket thread ends
  sched:run()
  assertTrue(t.continue)
end

function should.runWithZmqSocket(t)
  -- This test also ensures that created threads are run in creation order
  -- (server first, then client).
  local sched = lk.Scheduler()
  lk.Thread.setScheduler(sched)
  local port
  local srv = zmq.Push()
  local thread = lk.Thread(function()
    srv:send('Hello Lubyk!')
  end)

  local cli
  cli = zmq.SimplePull(srv:port(), function(...)
    local data = ...
    assertEqual('Hello Lubyk!', data)
    t.continue = true
    cli:quit()
  end)
  
  -- runs until socket thread ends
  sched:run()
  assertTrue(t.continue)
end

test.all()

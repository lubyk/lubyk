--[[------------------------------------------------------

  lk.Process test
  ---------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.Process')
local timeout = 3000

function should.loadCode()
  assertTrue(lk.Process)
end

function should.createPatch()
  local process = lk.Process(fixture.path('simple.yml'))
  assertType('table', process.nodes.add)
  assertType('table', process.nodes.store)
end

function should.announceService()
  local continue = false
  local browser = mdns.Browser(lubyk.service_type, function(service)
    if service.op == 'add' and service.name == 'simple' then
      continue = true
    end
  end)
  -- wait (and give time for callback to enter Lua State)
  local now = worker:now()
  while not continue and worker:now() < now + timeout do
    sleep(10)
  end
  assertTrue(continue)
end
test.all()

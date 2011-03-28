--[[------------------------------------------------------

  lk.Process test
  ---------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.Process')
local timeout = 1000

function should.load_code()
  assert_true(lk.Process)
end

function should.create_patch()
  local process = lk.Process(fixture.path('simple.yml'))
  assert_type('table', process.nodes.add)
  assert_type('table', process.nodes.store)
end

function should.announce_service()
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
  assert_true(continue)
end
test.all()

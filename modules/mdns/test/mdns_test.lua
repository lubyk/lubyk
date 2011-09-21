--[[------------------------------------------------------

  test mDNS for Lua
  -----------------

  mDNS implementation with callbacks to integrate with
  lubyk.

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('mdns')
local timeout = 5000

function should_register()
  local continue = false
  -- register our service at port 12345
  local registration = mdns.Registration(lubyk.service_type, 'Service1', 12345, function()
    continue = true
  end)
  local now = worker:now()
  while not continue and worker:now() < now + timeout do
    sleep(10)
  end
  assertTrue(continue)
end

function should.browse(t)
  local continue    = 0
  local should_op   = 'add'
  local hostname    = nil
  local name1       = 'Rucola'
  local name2       = 'Doors'
  local device_list = {}
  -- register a service at port 12345
  local registration = mdns.Registration(lubyk.service_type, name1, 12346, function(service)
    hostname = service.host
    continue = continue + 1
  end)

  local registration2 = mdns.Registration(lubyk.service_type, name2, 12346, function(service)
    hostname = service.host
    continue = continue + 1
  end)

  t:timeout(5000, function(done)
    if done or continue == 2 then
      assertEqual(2, continue)
      return true
    end
  end)
  continue = 0

  local browser = mdns.Browser(lubyk.service_type, function(self, service)
    if service.op == should_op and
      (service.name == name1 or
      service.name == name2) then
      continue = continue + 1
    end
  end)

  -- wait (and give time for callback to enter Lua State)
  t:timeout(3000, function(done)
    if done or continue == 2 then
      assertEqual(2, continue)
      return true
    end
  end)

  continue   = 0
  should_op  = 'remove'
  registration = nil  -- delete
  registration2= nil
  collectgarbage('collect')

  -- wait (and give time for callback to enter Lua State)
  t:timeout(3000, function(done)
    if done or continue == 2 then
      assertEqual(2, continue)
      return true
    end
  end)
end

test.all()

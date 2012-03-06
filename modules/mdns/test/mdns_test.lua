--[[------------------------------------------------------

  test mDNS for Lua
  -----------------

  mDNS implementation with callbacks to integrate with
  lubyk.

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('mdns')
local TIMEOUT = 5000

function should.register(t)
  local continue = false
  -- register our service at port 12345
  local registration = mdns.Registration(lubyk.service_type, 'Service1', 12345, function()
    continue = true
  end)
  t:timeout(TIMEOUT, function()
    return continue
  end)
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

  t:timeout(TIMEOUT, function()
    return continue == 2
  end)
  assertEqual(2, continue)
  continue = 0

  local browser = mdns.Browser(lubyk.service_type, function(self, service)
    if service.op == should_op and
      (service.name == name1 or
      service.name == name2) then
      port = service.port
      continue = continue + 1
    end
  end)

  -- wait (and give time for callback to enter Lua State)
  t:timeout(TIMEOUT, function()
    return continue == 2
  end)

  assertEqual(2, continue)
  assertEqual(12346, port)

  continue   = 0
  should_op  = 'remove'
  registration.debug = lk.Finalizer(function()
    print("REMOVED", name1)
  end)
  registration2.debug = lk.Finalizer(function()
    print("REMOVED", name2)
  end)

  registration = nil  -- delete
  registration2= nil
  collectgarbage('collect')
  collectgarbage('collect')

  -- wait (and give time for callback to enter Lua State)
  t:timeout(TIMEOUT, function()
    return continue == 2
  end)
  assertEqual(2, continue)
end

function should.registerTxt(t)
  local continue = false
  -- register our service at port 12345
  local registration = mdns.Registration(lubyk.service_type, 'Service3', 12345, {planet='Dune', name='Worm', pull=34}, function(s)
    continue = true
  end)
  assertMatch(string.char(11) .. 'planet=Dune', registration.txt)
  assertMatch(string.char(9)  .. 'name=Worm',   registration.txt)
  assertMatch(string.char(7)  .. 'pull=34',     registration.txt)
  t:timeout(TIMEOUT, function()
    return continue
  end)

  continue = false
  local txt
  local browser = mdns.Browser(lubyk.service_type, function(self, service)
    if service.op == 'add' and service.name == 'Service3' then
      txt = service.txt
      print(yaml.dump(txt))
      continue = true
    end
  end)
  t:timeout(TIMEOUT, function()
    return continue
  end)
  assertEqual('Dune', txt.planet)
  assertEqual('Worm', txt.name)
  assertEqual(34, txt.pull)
end

test.all()

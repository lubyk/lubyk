--[[------------------------------------------------------

  lk.Process test
  ---------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.Process')

function should.loadCode()
  assertTrue(lk.Process)
end

function should.announceService(t)
  local process = lk.Process('simple')
  local continue = false
  local browser = mdns.Browser(lubyk.service_type)
  function browser:addDevice(service)
    if service.name == Lubyk.zone .. ':simple' then
      continue = true
    end
  end

  -- wait (and give time for callback to enter Lua State)
  t:timeout(3, function()
    return continue
  end)
  assertTrue(continue)
  process = nil
end

test.all()

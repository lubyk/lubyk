--[[------------------------------------------------------

  test mDNS for Lua
  -----------------

  This file ends with '_test_' so that it is not run with
  all the tests.

--]]------------------------------------------------------
require 'rubyk'

local should = test.Suite('mDNS')
local device_list = {}

function should.register()
  -- register our service at port 12345
  local registration = mdns.Registration('My Super Service Name', rk.service_type, 12345)
  assert(registration.active)
  -- registration will be removed when 'registration' is garbage collected
end

function browse_callback(service)
  if service.remove then
    device_list[service.name] = nil
  else
    device_list[service.name] = service
  end
end

function should.browse()
  -- register a service at port 12345
  local registration = mdns.Registration('My Super Service Name', rk.service_type, 12345)
  local browser = mdns.Browser(rk.service_type, browse_callback)
  -- wait (and give time for callback to enter Lua State)
  rk.sleep(3000) -- ms
  -- browser should see registration
  assert(device_list['My Super Service Name'])
  -- registration will be removed when 'registration' is garbage collected
end
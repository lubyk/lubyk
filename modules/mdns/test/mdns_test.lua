--[[------------------------------------------------------

  test mDNS for Lua
  -----------------

  mDNS implementation with callbacks to integrate with
  rubyk.

--]]------------------------------------------------------
require 'rubyk'

local should = test.Suite('mdns')

function should.register()
  local continue = false
  -- register our service at port 12345
  local registration = mdns.Registration(rubyk.service_type, 'Service1', 12345, function()
    continue = true
  end)
  while not continue do
    worker:sleep(10)
  end
  -- if it does not hang: it works.
  assert_true(true)
end

function should.browse()
  local continue = false
  local condition = 'add'
  local device_list = {}
  -- register a service at port 12345
  local registration = mdns.Registration(rubyk.service_type, 'Service for browse', 12346, function()
    continue = true
  end)
  -- wait (and give time for callback to enter Lua State)
  while not continue do
    worker:sleep(10)
  end
  continue = false

  local browser = mdns.Browser(rubyk.service_type, function(what, service)
    if what == condition and service == 'Service for browse' then
      continue = true
    end
  end)
  -- wait (and give time for callback to enter Lua State)
  while not continue do
    worker:sleep(10)
  end

  continue  = false
  condition = 'remove'
  registration:__gc() -- explicit kill
  registration = nil  -- dangling pointer

  -- wait (and give time for callback to enter Lua State)
  while not continue do
    worker:sleep(10)
  end
  -- if it does not hang: it works.
  assert_true(true)
end

test.all()
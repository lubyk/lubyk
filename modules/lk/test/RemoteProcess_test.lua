--[[------------------------------------------------------

  lk.RemoteProcess test
  ---------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.RemoteProcess')

function should.loadCode()
  assertTrue(lk.RemoteProcess)
end

function should.createDummy()
  local process = lk.RemoteProcess('foo')
  assertEqual('lk.RemoteProcess', process.type)
  assertEqual('foo', process.name)
end


test.all()

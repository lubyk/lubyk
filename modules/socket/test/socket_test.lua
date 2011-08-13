--[[------------------------------------------------------

  socket test
  -----------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('socket')

function should.autoLoad()
  assertTrue(socket)
end

function should.autoloadHttp()
  assertType('table', socket.http)
end
test.all()

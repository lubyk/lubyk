--[[------------------------------------------------------

  lk.DavServer test
  -----------------

  ...

--]]------------------------------------------------------
require 'lubyk'
local should = test.Suite('lk.DavServer')
local crypto = lk.crypto

function should.autoLoad()
  assertTrue(lk.DavServer)
end

test.all()


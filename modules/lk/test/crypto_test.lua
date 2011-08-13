--[[------------------------------------------------------

  lk.crypto test
  --------------

  ...

--]]------------------------------------------------------
require 'lubyk'
local should = test.Suite('lk.crypto')
local crypto = lk.crypto

function should.autoLoad()
  assertTrue(lk.crypto)
end

function should.hashString()
  assertEqual('531d5289be744a01ff4736dfce072061b5087a59', crypto.sha1('This is just some random string'))
  assertEqual('531d5289be744a01ff4736dfce072061b5087a59', crypto.sha1('This is just some random string'))
end
test.all()

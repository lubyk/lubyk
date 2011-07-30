--[[------------------------------------------------------

  lk.Mutex test
  -------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.Mutex')

function should.loadCode()
  assertTrue(lk.Mutex)
end

function should.lockOnce()
  m = lk.Mutex()
  m:lock()
  assertEqual(1, m:lockCount())
  m:unlock()
end

function should.lockMany()
  m = lk.Mutex()
  for i=1,10 do
    m:lock()
  end
  assertEqual(10, m:lockCount())
  for i=1,10 do
    m:unlock()
  end
end

-- we cannot test proper locking from other thread because of the Gil on Lua state.

test.all()

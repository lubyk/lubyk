--[[------------------------------------------------------

  gc_test
  -------

  Make sure objects with Lua threads (callbacks) and other
  complex objects (related to memory management) are totally
  removed on garbage collection.

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.Thread')

function should.remove_thread()
  local removed = false
  local thread = lk.Thread(function()
    --
  end)
  thread.f = lk.Finalizer(function()
    removed = true
  end)
  thread = nil
  collectgarbage('collect')
  assertTrue(removed)
end

test.all()

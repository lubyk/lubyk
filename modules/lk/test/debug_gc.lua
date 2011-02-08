--[[------------------------------------------------------

  gc_test
  -------

  Make sure objects with Lua threads (callbacks) and other
  complex objects (related to memory management) are totally
  removed on garbage collection.

--]]------------------------------------------------------
require 'lubyk'

--local should = test.Suite('lk.Thread')
--
--function should.remove_thread()
  thread = lk.Thread(function()
    --
  end)
  thread = nil
  collectgarbage('collect')
  io.flush()
  print('done')
--end

--test.all()
--[[------------------------------------------------------

  Runner
  ------

  The Runner library is the starting point for running
  patches or running tests. The Runner does not alter
  the search path: this is done by the 'lubyk' executable.

--]]------------------------------------------------------

print('Loading Runner...')
local lib = {}
lk.Runner = lib

--- Runs all tests in the 'test' directory of the current path.
-- Any file ending in '_test.lua' is considered a test and is
-- executed. The tests share nothing between files (a new process
-- is run for each file.)
function lib.test_all()
  require 'lunit/lunit'
  print('Running all tests')
end

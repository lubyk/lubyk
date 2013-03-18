--[[------------------------------------------------------

  lk.Dir test
  --------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.Dir')

function should.listFilesMatchingPattern()
  local dir = lk.Dir('modules/lubyk')
  local pattern = '%.lua$'
  for file in dir:glob(pattern) do
    assertMatch(pattern, file)
  end
end

test.all()

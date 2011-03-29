--[[------------------------------------------------------

  lubyk.Metro test
  ----------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lubyk.Metro')

function should.build_in_patch()
  local patch = lk.Patch(fixture.path('metro.yml'))
  assertEqual(100, patch.nodes.beat.env.tempo)
end

test.all()

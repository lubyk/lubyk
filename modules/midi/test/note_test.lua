--[[------------------------------------------------------

  midi.Note test
  --------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('midi.Note')

function should.autoLoad()
  assertTrue(midi.Note)
end

function should.mapMiddleC()
  assertEqual(60, midi.Note['C4'])
end
test.all()

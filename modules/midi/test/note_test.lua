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

function should.reverseMap()
  assertEqual('C4', midi.Note.name[60])
end
test.all()

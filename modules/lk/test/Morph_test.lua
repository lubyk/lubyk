--[[------------------------------------------------------

  lk.Morph test
  -------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.Morph')

function should.loadCode()
  assertTrue(lk.Morph)
end

function should.createEmptyFile()
  local lkp = fixture.path('empty.lkp')
  lk.rmFile(lkp)
  assertFalse(lk.exist(lkp))
  local morph = lk.Morph(lkp)
  assertTrue(lk.exist(lkp))
  lk.rmFile(lkp)
end

function should.readLKPFile()
  local lkp = fixture.path('project/example.lkp')
  local morph = lk.Morph(lkp)
test.all()

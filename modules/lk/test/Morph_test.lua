--[[------------------------------------------------------

  lk.Morph test
  -------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.Morph')

local function makeMorph(filepath)
  local morph = lk.Morph()
  morph.spawn = function() end -- do not spawn when testing
  if filepath then
    morph:open(filepath)
  end
  return morph
end

function should.loadCode()
  assertTrue(lk.Morph)
end

function should.createEmptyFile()
  local lkp = fixture.path('empty.lkp')
  lk.rmFile(lkp)
  assertFalse(lk.exist(lkp))
  local morph = makeMorph(lkp)
  assertTrue(lk.exist(lkp))
  lk.rmFile(lkp)
end

function should.readLkpFile()
  local lkp = fixture.path('project/example.lkp')
  local morph = makeMorph(lkp)
  assertEqual(0, morph.lubyk.version.major)
  assertEqual(3, morph.lubyk.version.minor)
  assertEqual('saturn', morph.processes.foobar.host)
end
  
function should.saveToFile()
  local lkp = fixture.path('empty.lkp')
  lk.rmFile(lkp)
  local morph = makeMorph(lkp)
  morph.processes.hello = {host='waga', dir='hello'}
  morph.private.writeFile(morph)
  -- should have created empty.lkp with all data
  local data = yaml.loadpath(lkp)
  assertEqual(0, data.lubyk.version.major)
  assertEqual(5, data.lubyk.version.minor)
  assertEqual('waga', data.processes.hello.host)
  lk.rmFile(lkp)
end

function should.createFilesOnNewProcess()
  local lkp = fixture.path('empty.lkp')
  local hello = fixture.path('hello')
  lk.rmFile(lkp)
  lk.rmTree(hello)
  local morph = makeMorph(lkp)
  morph.private.process.add(morph, 'hello', {host='waga'})
  -- should create 'hello' directory and 'hello/_patch.yml'
  assertEqual('directory', lk.fileType(hello))
  assertEqual('file', lk.fileType(hello .. '/_patch.yml'))
  -- should have written changes to empty.lkp
  local data = yaml.loadpath(lkp)
  assertEqual(0, data.lubyk.version.major)
  assertEqual(5, data.lubyk.version.minor)
  assertEqual('waga', data.processes.hello.host)
  lk.rmFile(lkp)
  lk.rmTree(hello)
end

function should.dump()
  local lkp = fixture.path('project/example.lkp')
  local morph = makeMorph(lkp)
  local data = morph.private.dumpAll(morph)
  assertEqual(0, data.lubyk.version.major)
  assertEqual(3, data.lubyk.version.minor)
  assertEqual('', data.processes.bang)
  assertEqual('inline', data.processes.foobar.dir)
end

test.all()


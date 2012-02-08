--[[------------------------------------------------------

  lk.Settings test
  ----------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.Settings')

local defaults = {
  name = 'Buzz',
  list = { 'a','b','c' },
}

local settings = lk.Settings('TestSettings', defaults)

function should.readDefaultValues()
  local s = lk.Settings('TestSettings', defaults)
  assertEqual('Buzz', s.name)
  assertEqual('a', s.list[1])
end

function should.readChangedSettings()
  local s = lk.Settings('TestSettings', defaults)
  assertMatch('return {}', lk.Settings.dump(s))
end

function should.dumpSettings()
  local s = lk.Settings('TestSettings', defaults)
  assertMatch('return {}', lk.Settings.dump(s))
end

function should.dumpChangedSettings()
  local s = lk.Settings('TestSettings', defaults)
  s.new_key = 'Hop'
  assertMatch('return {.*new_key = "Hop",', lk.Settings.dump(s))
end

function should.dumpStringWithQuote()
  local s = lk.Settings('TestSettings', defaults)
  s.angle = '24"'
  assertMatch('return {.*angle = "24\\"",', lk.Settings.dump(s))
end

function should.dumpDeepSetting()
  local s = lk.Settings('TestSettings', defaults)
  s.list = {'e','f', g = 'h'}
  assertMatch('return {.*list = {.*%[1%] = "e",.*g = "h"', lk.Settings.dump(s))
end

function should.readSettingsFromFile()
  lk.writeall(settings.module.path, [[
  return {
    name = 'John',
    age  = 22,
  }
  ]])
  local s = lk.Settings('TestSettings', defaults)
  assertEqual('John', s.name)
  assertEqual(22, s.age)
end

function should.saveSettings()
  local s = lk.Settings('TestSettings', defaults)
  s.name = 'Fuzz'
  s.list = {1,2,'a'}
  assertFalse(lk.exist(settings.module.path))
  s:save()
  assertTrue(lk.exist(settings.module.path))
  local dump = lk.readall(settings.module.path)
  assertMatch('name = "Fuzz"', dump)
  assertMatch('list = {.*%[1%] = 1', dump)
end

function should.saveValidLua()
  local s = lk.Settings('TestSettings', defaults)
  s.name = 'Fuzz'
  s.list = {1,2,'a'}
  s:save()
  local t = loadfile(settings.module.path)()
  assertEqual('Fuzz', t.name)
  assertEqual('a', t.list[3])
end

function should.teardown()
  local path = settings.module.path
  if lk.exist(path) then
    lk.rmFile(path)
  end
end

test.all()

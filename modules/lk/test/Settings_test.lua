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
  foo = {
    bar = 'Baz',
  }
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

function should.notDumpModuleName()
  local s = lk.Settings('TestSettings', defaults)
  s.new_key = 'Hop'
  assertNotMatch('_module', lk.Settings.dump(s))
end

function should.dumpStringWithQuote()
  local s = lk.Settings('TestSettings', defaults)
  s.angle = '24"'
  assertMatch('return {.*angle = "24\\"",', lk.Settings.dump(s))
end

function should.copyOnWrite()
  local s = lk.Settings('TestSettings', defaults)
  s.foo.ban = 'Boom'
  -- Default in table copied
  assertMatch('return {.*foo = {.*bar = "Baz"', lk.Settings.dump(s))
  -- Custom value
  assertMatch('return {.*foo = {.*ban = "Boom"', lk.Settings.dump(s))
end

function should.copyOnWriteInSameTable()
  local s = lk.Settings('TestSettings', defaults)
  local f = s.foo
  f.ban = 'Boom'
  f.bar = 'Biz'
  -- Default in table copied
  assertMatch('return {.*foo = {.*bar = "Biz"', lk.Settings.dump(s))
  -- Custom value
  assertMatch('return {.*foo = {.*ban = "Boom"', lk.Settings.dump(s))
end

function should.readSettingsFromFile()
  lk.writeall(settings._module.path, [[
  return {
    name = 'John',
    age  = 22,
    foo  = { bar = 'Biz', ban = 'Boom' },
  }
  ]])
  local s = lk.Settings('TestSettings', defaults)
  assertEqual('John', s.name)
  assertEqual(22, s.age)
  assertEqual('Biz', s.foo.bar)
end

function should.saveSettings()
  local s = lk.Settings('TestSettings', defaults)
  s.name = 'Fuzz'
  s.foo.ban = 'Boom'
  assertFalse(lk.exist(settings._module.path))
  s:save()
  assertTrue(lk.exist(settings._module.path))
  local dump = lk.readall(settings._module.path)
  assertMatch('name = "Fuzz"', dump)
  assertMatch('foo = {.*ban = "Boom"', dump)
end

function should.saveValidLua()
  local s = lk.Settings('TestSettings', defaults)
  s.name = 'Fuzz'
  s.foo.ban = 'Boom'
  s:save()
  local t = loadfile(settings._module.path)()
  assertEqual('Fuzz', t.name)
  assertEqual('Boom', t.foo.ban)
  assertEqual('Baz',  t.foo.bar)
end

function should.saveTrueFalse()
  local s = lk.Settings('TestSettings', defaults)
  s.name = true
  s.foo.ban = false
  s:save()
  local t = loadfile(settings._module.path)()
  assertEqual(true, t.name)
  assertEqual(false, t.foo.ban)
end

function should.teardown()
  local path = settings._module.path
  if lk.exist(path) then
    lk.rmFile(path)
  end
end

test.all()

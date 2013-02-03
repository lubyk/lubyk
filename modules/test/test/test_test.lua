require 'lubyk'
require 'test'

local should = test.Suite('test')
local run = ''
local base = lk.scriptDir()

function should.runTest()
  run = run .. '1'
  assertEqual(1, 1)
end

function should.sayHello()
  run = run .. ',2'
  assertEqual('887', '887')
  assertMatch('true', 'This is true')
end

function should.findFixtures()
  run = run .. ',3'
  assertEqual(base .. '/fixtures/aaa.txt', fixture.path('aaa.txt'))
end

function should.readFixtures()
  run = run .. ',4'
  assertEqual('hello test\n', fixture.content('aaa.txt'))
end

function should.compareTables()
  run = run .. ',5'
  assertValueEqual(
    {1, {a='a'}},
    {1, {a='a'}}
  )
end

function should.compareWithResolution()
  run = run .. ',6'
  assertValueEqual(1, 1.5, 0.5)
  assertEqual(1, 1.5, 0.5)
  assertValueEqual({1, 2}, {1.5, 1.5}, 0.5)
end

function should.runInOrder()
  run = run .. ',7'
  assertEqual('1,2,3,4,5,6,7', run)
end

test.all()

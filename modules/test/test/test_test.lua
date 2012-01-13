require 'lubyk'
require 'test'

local should = test.Suite('test')
local run = ''

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
  assertMatch('modules/test/test/fixtures/aaa', fixture.path('aaa'))
end

function should.compareTables()
  run = run .. ',4'
  assertValueEqual(
    {1, {a='a'}},
    {1, {a='a'}}
  )
end

function should.compareWithResolution()
  run = run .. ',5'
  assertValueEqual(1, 1.5, 0.5)
  assertEqual(1, 1.5, 0.5)
  assertValueEqual({1, 2}, {1.5, 1.5}, 0.5)
end

function should.runInOrder()
  run = run .. ',6'
  assertEqual('1,2,3,4,5,6', run)
end

test.all()

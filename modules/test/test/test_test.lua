require 'lubyk'
require 'test'

local should = test.Suite('test')

function should.runTest()
  assertEqual(1, 1)
end

function should.sayHello()
  assertEqual('887', '887')
  assertMatch('true', 'This is true')
end

function should.findFixtures()
  assertMatch('modules/test/test/fixtures/aaa', fixture.path('aaa'))
end

function should.compareTables()
  assertValueEqual(
    {1, {a='a'}},
    {1, {a='a'}}
  )
end

function should.compareWithResolution()
  assertValueEqual(1, 1.5, 0.5)
  assertEqual(1, 1.5, 0.5)
  assertValueEqual({1, 2}, {1.5, 1.5}, 0.5)
end

test.all()

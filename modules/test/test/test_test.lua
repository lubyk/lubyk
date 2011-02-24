require 'lubyk'
require 'test'

local should = test.Suite('test')

function should.add_inlets()
  assert_equal(1, 1)
end

function should.say_hello()
  assert_equal('887', '887')
  assert_match('true', 'This is true')
end

function should.find_fixtures()
  assert_equal('887', test.fixture('aaa'))
end

test.all()
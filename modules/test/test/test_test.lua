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

test.all()
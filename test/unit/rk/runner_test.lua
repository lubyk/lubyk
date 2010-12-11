require 'rubyk'
require 'rk.Test'

local should = rk.Test.Suite('Basic tests')

function should.add_inlets()
  assert_equal(2, 1)
end

function should.say_hello()
  assert_equal('887', '887')
  assert_match('true', 'This is true')
end

rk.Test.all()
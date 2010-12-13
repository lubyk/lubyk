require 'rubyk'
require 'rk.Test'

local should = rk.Test.Suite('rk.Test')

function should.add_inlets()
  assert_equal(1, 1)
end

function should.say_hello()
  assert_equal('887', '887')
  assert_match('true', 'This is true')
end

rk.Test.all()
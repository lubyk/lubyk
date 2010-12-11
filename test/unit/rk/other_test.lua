require 'rubyk'
require 'rk.Test'

local should = rk.Test.Suite('Other tests')

function should.add_ff()
  assert_equal(1, 1)
end

function should.say_hello()
  assert_equal('887', '887')
end
rk.Test.all()
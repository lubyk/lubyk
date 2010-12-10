require 'rubyk'
require 'rk/test'

local basic = rk.Test.Suite('Basic tests')

function basic.should_add_inlets()
  assert_equal(1, 1)
end

function basic.should_say_hello()
  assert_equal('887', '887')
end

local other = rk.Test.Suite('Other tests')

function other.should_add_ff()
  assert_equal(1, 1)
end

function other.should_say_hello()
  assert_equal('887', '887')
end
rk.Test.all()
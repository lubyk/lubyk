--[[------------------------------------------------------

  Basic tests for OpenGL
  ----------------------

  ...

--]]------------------------------------------------------
require 'lubyk'
local should = test.Suite('gl (TODO)')

function should.load_gl()
  assert_true(gl)
end

test.all()
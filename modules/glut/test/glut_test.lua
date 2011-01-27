--[[------------------------------------------------------

  Basic tests for OpenGL
  ----------------------

  ...

--]]------------------------------------------------------
require 'lubyk'
local should = test.Suite('glut (TODO)')

function should.load_glut()
  assert_true(glut)
end

test.all()
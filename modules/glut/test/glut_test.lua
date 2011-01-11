--[[------------------------------------------------------

  Basic tests for OpenGL
  ----------------------

  ...

--]]------------------------------------------------------
require 'rubyk'
local should = test.Suite('glut (TODO)')

function should.load_glut()
  assert_true(glut)
end

test.all()
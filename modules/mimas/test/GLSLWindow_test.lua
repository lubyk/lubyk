--[[------------------------------------------------------

  Basic tests for modern OpenGL window
  ------------------------------------

  This is for modern, shader based OpenGL.

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('mimas.GLSLWindow')
local withUser = should:testWithUser()

function should.autoload()
  assertTrue(mimas.GLSLWindow)
end

-- function should.createGl3Context()
--   local w = mimas.GLSLWindow()
--   w:show() -- this creates the OpenGL
--   local gl_version, glsl_version = w:openGLVersion()
--   print(gl_version, glsl_version)
--   assertMatch('3.2', gl_version)
-- end

function should.displayGlWindow(t)
  -- we use the test env to protect from gc
  t.win = mimas.GLSLWindow()
  t.win:move(300,300)
  t.win:resize(400,400)
  t.win:show()
  local gl_version, glsl_version = t.win:openGLVersion()
  print(gl_version, glsl_version)
  t.win:compile(
-- Vertex shader
[=[
#version 150
layout(location=0) in vec4 in_Position;
layout(location=1) in vec4 in_Color;
out vec4 ex_Color;

void main(void)
{
  gl_Position = in_Position;
  ex_Color = in_Color;
}
]=],

-- Fragment shader
[=[
#version 150
 
in vec4 ex_Color;
out vec4 out_Color;
 
void main(void)
{
  out_Color = ex_Color;
}
]=]
)
sleep(100000)
end

function should.acceptDestroyFromGui(t)
  t.win = mimas.GLSLWindow()
  t.win:move(100, 170)
  t.win:resize(50, 50)
  t.win:show()

  t.thread = lk.Thread(function()
    sleep(200)
    t.win:close()
    while not t.win:deleted() do
      sleep(200)
    end
    -- should be deleted by GUI
    assertMatch('NULL', t.win:__tostring())
  end)
end

function should.acceptDestroyFromLua()
  local win = mimas.GLSLWindow()
  win:move(100, 240)
  win:resize(50, 50)
  win:show()
  local label = mimas.Label("Hop", win)

  thread = lk.Thread(function()
    win = nil
    collectgarbage('collect')
    -- not deleted by Lua, but marked as deleted in C++
    -- proof that win was deleted in C++
    assertTrue(label:deleted())
  end)
end

test.all()


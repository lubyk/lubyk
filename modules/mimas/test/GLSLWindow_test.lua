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

function should.createGl3Context()
  local w = mimas.GLSLWindow()
  w:show() -- this creates the OpenGL
  local gl_version, glsl_version = w:openGLVersion()
  print(gl_version, glsl_version)
  assertMatch('3.2', gl_version)
end

function withUser.should.displayGlWindow(t)
  -- we use the test env to protect from gc
  t.win = mimas.GLSLWindow()
  t.win:move(300,300)
  t.win:resize(400,400)
  function t.win:initializeGL()
    -- gl.Enable("POINT_SMOOTH")
    -- gl.Enable("SMOOTH")
    -- gl.Enable("BLEND")                                -- Enable alpha blending
    -- gl.BlendFunc("SRC_ALPHA", "ONE_MINUS_SRC_ALPHA")  -- Set blend function

    -- gl.ClearDepth(1.0)
    -- gl.DepthFunc("LEQUAL");

    -- -- gl.Enable('GL_CULL_FACE')
    -- -- gl.Enable('GL_DEPTH_TEST')

    -- gl.Hint("PERSPECTIVE_CORRECTION_HINT", "NICEST") -- Really nice perspective
    -- gl.ClearColor(0.2,0.2,0.2,0.5)
  end

  function t.win:resizeGL(width, height)
    -- gl.Enable("BLEND")
    -- --gl.Disable("DEPTH_TEST")
    -- gl.BlendFunc("SRC_ALPHA", "ONE_MINUS_SRC_ALPHA")

    -- gl.Enable("LINE_SMOOTH")
    -- -- Select the projection matrix
    -- gl.MatrixMode("PROJECTION")
    -- -- reset
    -- gl.LoadIdentity()
    -- -- Calculate the aspect ratio of the view
    -- glu.Perspective(
    --   45,               -- Field of view angle
    --   width / height,   -- Aspect ration
    --   1,                -- zNear
    --   100               -- zFar
    -- )
  end
  t.n = 0
  t.x = 0
  t.y = 0
  t.z = 0
  t.dt = (math.pi / 100)
  t.last = worker:now()

  t.fps  = 0
  local AVG = 100
  local function computeFps(now)
    local fps = 1000 / (now - t.last)
    t.fps = (AVG * t.fps + fps) / (AVG+1)
    return t.fps
  end

  local function animate(now)
    local dt = (now - t.last) * t.dt
    t.n = t.n + t.dt
    t.x = math.cos(t.n / 0.9) * 360 / math.pi
    t.y = math.sin(t.n / 0.7) * 360 / math.pi
    t.z = math.sin(t.n) * 360 / math.pi
    t.label:setText(string.format('fps: %.2f', computeFps(now)))
    t.last = now
  end

  function t.win:paintGL()
    -- animate(worker:now())
    -- gl.Clear( "COLOR_BUFFER_BIT, DEPTH_BUFFER_BIT")
    -- gl.MatrixMode("MODELVIEW")
    -- gl.LoadIdentity()

    -- gl.Translate(0.0, 0.0, -6.0)

    -- gl.Rotate(t.x, 1.0, 0.0, 0.0)
    -- gl.Rotate(t.y, 0.0, 1.0, 0.0)
    -- gl.Rotate(t.z, 0.0, 0.0, 1.0)

    -- gl.Color(0.5,0.5,0.0,0.3)
    -- gl.LineWidth(1.0)
    -- glut.WireCube(2.6)

    -- gl.Color(0.5,0.5,0.0,0.1)
    -- glut.SolidCube(2.6)
    -- t.win:updateGL()
  end

  t.btn = mimas.Button('ok', function()
    t.continue = true
  end)
  t.win:addWidget(t.btn, 10, 10)
  t.label = mimas.Label('Hello')

  t.shape = mimas.Widget()
  t.win:addWidget(t.shape, 100, 100)
  t.shape:resize(100, 100)
  function t.shape:paint(p, w, h)
    p:setBrush(mimas.Color(0, 0.8, 0.8, 0.3))
    p:setPen(3, mimas.Color(0.3, 0.8, 0.8, 0.5))
    p:drawRoundedRect(5, 5, 90, 90, 10, 10)
  end
  t.shape:setStyle('background:transparent')
  t.shape:addWidget(t.label, 20, 40)
  t.label:resize(100)

  t.win:resize(300,300)
  t.win:show()
  t:timeout(34000, function(done)
    return done or t.continue
  end)
  t.win:close()
  assertTrue(t.continue)
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


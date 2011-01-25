--[[------------------------------------------------------

  Basic tests for rubyk GUI
  -------------------------

  This tests multi-threading and event posting / triggering.
  We cannot trigger these tests with other tests because Qt
  does not like beeing started and stopped multiple times.

--]]------------------------------------------------------
require 'rubyk'

local should = test.Suite('mimas.GLWindow')

function should.display_window(t)
  -- we use the test env to protect from gc
  t.win = mimas.GLWindow()
  function t.win.initializeGL()
    gl.Enable("POINT_SMOOTH")
    gl.Enable("SMOOTH")
    gl.Enable("BLEND")                                -- Enable alpha blending
    gl.BlendFunc("SRC_ALPHA", "ONE_MINUS_SRC_ALPHA")  -- Set blend function

    gl.ClearDepth(1.0)
    gl.DepthFunc("LEQUAL");

    -- glEnable(GL_CULL_FACE);
    -- glEnable(GL_DEPTH_TEST);

    gl.Hint("PERSPECTIVE_CORRECTION_HINT", "NICEST") -- Really nice perspective
    gl.ClearColor(0.2,0.2,0.2,0.5)
  end

  function t.win.resizeGL(width, height)
    gl.Enable("BLEND")
    gl.Disable("DEPTH_TEST")
    gl.BlendFunc("SRC_ALPHA", "ONE_MINUS_SRC_ALPHA")

    gl.Enable("LINE_SMOOTH")
    -- Select the projection matrix
    gl.MatrixMode("PROJECTION")
    -- reset
    gl.LoadIdentity()
    -- Calculate the aspect ratio of the view
    gl.Perspective(
      45,               -- Field of view angle
      width / height,   -- Aspect ration
      1,                -- zNear
      100               -- zFar
    )
  end
  t.n = 0
  t.x = 0
  t.y = 0
  t.z = 0
  t.dt = (math.pi / 100)
  t.now = worker:now()

  t.timer = rk.Timer(20, function()
    t.n = t.n + t.dt
    t.x = math.cos(t.n / 0.9) * 360 / math.pi
    t.y = math.sin(t.n / 0.7) * 360 / math.pi
    t.z = math.sin(t.n) * 360 / math.pi
    t.win:updateGL()
    if worker:now() > t.now + 1500 then
      app:post(function()
        t.timer:stop()
        -- proves that the window was open and all is fine
        assert_true(t.win:close())
      end)
    end
  end)
  t.timer:start()

  function t.win.paintGL()
    gl.Clear( "COLOR_BUFFER_BIT, DEPTH_BUFFER_BIT")
    gl.MatrixMode("MODELVIEW")
    gl.LoadIdentity()

    gl.Translate(0.0, 0.0, -6.0)

    gl.Rotate(t.x, 1.0, 0.0, 0.0)
    gl.Rotate(t.y, 0.0, 1.0, 0.0)
    gl.Rotate(t.z, 0.0, 0.0, 1.0)

    gl.Color(0.5,0.5,0.0,0.3)
    gl.LineWidth(1.0)
    glut.WireCube(2.6)

    gl.Color(0.5,0.5,0.0,0.1)
    glut.SolidCube(2.6)
  end

  t.win:show()
end

function should.accept_destroy_from_gui(t)
  t.win = mimas.GLWindow()
  t.win:move(100, 170)
  t.win:resize(50, 50)
  t.win:show()

  t.thread = rk.Thread(function()
    sleep(200)
    t.win:close()
    while not t.win:deleted() do
      sleep(200)
    end
    -- should be deleted by GUI
    assert_match('NULL', t.win:__tostring())
  end)
end

function should.accept_destroy_from_Lua()
  local win = mimas.GLWindow()
  win:move(100, 240)
  win:resize(50, 50)
  win:show()
  local label = mimas.Label("Hop", win)

  thread = rk.Thread(function()
    win = nil
    collectgarbage('collect')
    -- not deleted by Lua, but marked as deleted in C++
    -- proof that win was deleted in C++
    assert_true(label:deleted())
  end)
end

test.all()
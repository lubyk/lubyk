--[[------------------------------------------------------

  Use the wii.Remote to control OpenGL
  ------------------------------------

  Start the script and then press buttons '1' and '2' on
  the Remote to connect. The cube rotates depending on the
  data from the x, y and z axis. Press '1' or '2' to
  change the color, press '-' or '+' to change
  transparency. Press 'A' to start recording, 'A' again to
  stop and playback recorded data in a loop.

  Commands summary:

  1+2: discovery
  1: red
  2: blue
  +: opaque
  -: transparent
  A: record - playback - stop - record ...
  Home: quit

--]]------------------------------------------------------
require 'lubyk'

app = mimas.Application()

x, y, z = 0, 0, 0
alpha = 0.5
color = {r=0.8, g=0.2, b=0.2}
size  = 2.6
a     = 0

-- cube animation (button B)
timer = lk.Timer(30, function()
  a = a + 9
  size = 1.6 + math.cos(math.pi * a / 180)
  win:updateGL()
  if a >= 360 then
    size = 2.6
    return 0
  end
end)

win = mimas.GLWindow()
function win.initializeGL()
  gl.Enable("POINT_SMOOTH")
  gl.Enable("SMOOTH")
  gl.Enable("BLEND")                                -- Enable alpha blending
  gl.BlendFunc("SRC_ALPHA", "ONE_MINUS_SRC_ALPHA")  -- Set blend function

  gl.ClearDepth(1.0)
  gl.DepthFunc("LEQUAL");

  -- glEnable(GL_CULL_FACE);
  -- glEnable(GL_DEPTH_TEST);

  gl.Hint("PERSPECTIVE_CORRECTION_HINT", "NICEST") -- Really nice perspective
  gl.ClearColor(0.2,0.2,0.2,0.01)
end

function win.resizeGL(width, height)
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

function win.paintGL()
  gl.Clear( "COLOR_BUFFER_BIT, DEPTH_BUFFER_BIT")
  gl.MatrixMode("MODELVIEW")
  gl.LoadIdentity()

  gl.Translate(0.0, 0.0, -6.0)

  gl.Rotate(x * 360, 1.0, 0.0, 0.0)
  gl.Rotate(y * 360, 0.0, 1.0, 0.0)
  gl.Rotate(z * 360, 0.0, 0.0, 1.0)

  gl.Color(0.5,0.5,0.5,0.8)
  gl.LineWidth(1.0)

  -- mini cube
  gl.Translate(0.0, -1.0, 0.0)
  glut.WireCube(0.6)
  gl.Translate(0.0, 1.0, 0.0)

  -- big cube
  gl.Translate(0.0, -(2.6 - size)/2, 0.0)
  glut.WireCube(size)
  gl.Color(color.r, color.g, color.b, alpha)
  glut.SolidCube(size)
end
win:show()

-- in memory database stream recorder
stream = db.Stream()
stream.loop = true

function process_acceleration(lx, ly, lz)
  x = x + (lx - x) * 0.1
  y = y + (ly - y) * 0.1
  z = z + (lz - z) * 0.1
  win:updateGL()
end

function stream.playback(row)
  if row.b then
    process_button(row.b)
  else
    process_acceleration(row.x, row.y, row.z)
  end
end

function process_button(btn)
  if btn == 'Remote.U' then
    -- red
    color = {r=0.8, g=0.2, b=0.2}
  elseif btn == 'Remote.L' then
    -- blue
    color = {r=0.2, g=0.2, b=0.8}
  elseif btn == 'Remote.R' then
    -- green
    color = {r=0.2, g=0.8, b=0.2}
  elseif btn == 'Remote.D' then
    -- pink
    color = {r=0.8, g=0.2, b=0.8}
  elseif btn == 'Remote.1' then
    -- orange
    color = {r=1.0, g=0.5, b=0.0}
  elseif btn == 'Remote.2' then
    -- purple
    color = {r=0.6, g=0.1, b=0.9}
  elseif btn == 'Remote.-' then
    alpha = alpha - 0.1
    if alpha < 0 then
      alpha = 0
    end
  elseif btn == 'Remote.B' then
    -- shrink and grow
    size = 2.6
    a = 0
    timer:start()
  elseif btn == 'Remote.+' then
    alpha = alpha + 0.1
    if alpha > 1.0 then
      alpha = 1.0
    end
  elseif btn == 'Remote.H' then
    app:quit()
  end
end

browser = wii.Browser(function(found_wii)
  wiimote = found_wii
  function wiimote.acceleration(device, lx, ly, lz)
    stream:rec{x = lx, y = ly, z = lz}
    if not stream.playing then
      process_acceleration(lx, ly, lz)
    end
  end

  function wiimote.button(btn, on)
    if on then
      if btn == 'Remote.A' then
        if stream.recording then
          print("Stop recording and start playback.")
          stream:rec_stop()
          stream:play()
        elseif stream.playing then
          print("Playback stop.")
          stream:stop()
        else
          print("Started recording...")
          stream:rec_start()
        end
      elseif not stream.playing then
        stream:rec{b=btn}
        process_button(btn)
      end
    end
    win:updateGL()
  end
end)

app:exec()

n = n or 0
x = x or 0
y = y or 0
z = z or 0
dt = dt or (math.pi / 1000)

function advance()
  n = n + dt
  x = math.cos(n / 0.9) * 360 / math.pi
  y = math.sin(n / 0.7) * 360 / math.pi
  z = math.sin(n) * 360 / math.pi
end
Inlet('advance', BangIO('just bang me'))

function speed(sig)
  if sig then
    dt = sig
  end
  return dt
end
Inlet('speed', RangeIO('Rotating speed', 0, math.pi / 500))

function draw()
  gl.Clear( "COLOR_BUFFER_BIT, DEPTH_BUFFER_BIT")
  gl.MatrixMode("MODELVIEW")
  gl.LoadIdentity()

  gl.Translate(0.0, 0.0, -6.0)

  gl.Rotate(x, 1.0, 0.0, 0.0)
  gl.Rotate(y, 0.0, 1.0, 0.0)
  gl.Rotate(z, 0.0, 0.0, 1.0)

  gl.Color(0.5,0.5,0.0,0.3)
  gl.LineWidth(1.0)
  glut.WireCube(2.6)

  gl.Color(0.5,0.5,0.0,0.1)
  glut.SolidCube(2.6)
end

function resize(width, height)
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
    45.0,             -- Field of view angle
    width / height,   -- Aspect ration
    1.0,              -- zNear
    100.0             -- zFar
  )
  view.width  = width
  view.height = height
end
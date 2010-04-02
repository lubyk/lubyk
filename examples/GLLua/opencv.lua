require('cv')

-- size must be a power of 2
size  = cv.Size(128,128)
res   = res   or cv.Mat()

n = n or 0
x = x or 0
y = y or 0
z = z or 0

function x(val)
  if val then
    x = val
  end
  return x
end
inlet('x', RangeIO(0, 360, "rotation angle in x axis"))

function y(val)
  if val then
    y = val
  end
  return y
end
inlet('y', RangeIO(0, 360, "rotation angle in y axis"))

function z(val)
  if val then
    z = val
  end
  return z
end
inlet('z', RangeIO(0, 360, "rotation angle in z axis"))

function video(frame)
  if not frame then return nil end
  cv.resize(frame, res, size, 0, 0, cv.INTER_LINEAR)
  frame_changed = true

  n = n + math.pi / 300
  x = math.cos(n / 0.9) * 360 / math.pi
  y = math.sin(n / 0.7) * 360 / math.pi
  z = math.sin(n) * 360 / math.pi
end
inlet('video', MatrixIO('send me images'))

function draw()
  gl.Clear( "COLOR_BUFFER_BIT, DEPTH_BUFFER_BIT")
  --gl.ClearDepth(1.0)
  gl.MatrixMode("MODELVIEW")
  gl.LoadIdentity()

  if frame_changed then
    rk.makeTexture(res, tex)
    frame_changed = false
  end

  gl.Translate(0.0, 0.0, -6.0)

  gl.Rotate(x, 1.0, 0.0, 0.0)
  gl.Rotate(y, 0.0, 1.0, 0.0)
  gl.Rotate(z, 0.0, 0.0, 1.0)


  gl.Color(0.5,0.5,0.0)--,0.3)
  gl.LineWidth(1.0)
  glut.WireCube(2.6)

  gl.Color(0.5,0.5,0.0,0.5)
  glut.SolidCube(2.6)

  -- simples way to draw a texture
  gl.Translate(0.0, 0.0, -1.3)
  -- do not forget to set a color or nothing will be drawn
  gl.Color(1,1,1,0.5)
  rk.drawTexture(tex, 1.3, 1.3, -1.3, -1.3)
end

function resize(width, height)
  textures = textures or gl.GenTextures(1)
  tex = textures[1]

  gl.Enable("BLEND")
  gl.Disable("DEPTH_TEST") -- not working ???
  gl.BlendFunc("SRC_ALPHA", "ONE_MINUS_SRC_ALPHA")

  gl.Enable("TEXTURE_2D")

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

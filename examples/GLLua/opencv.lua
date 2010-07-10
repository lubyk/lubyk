require('cv')

-- size must be a power of 2
size    = cv.Size(128,128)
v_small = v_small or cv.Mat()
v_res   = v_res or cv.Mat()

n = n or 0
x_ = x_ or 0
y_ = y_ or 0
z_ = z_ or 0
blur_ = blur_ or 1
opacity_ = opacity_ or 0.5
blur_size = blur_size or cv.Size(blur_, blur_)

function x(val)
  if val then
    x_ = val
  end
  return x_
end
inlet('x', RangeIO("Rotation angle in x axis.", 0, 360))

function y(val)
  if val then
    y_ = val
  end
  return y_
end
inlet('y', RangeIO("Rotation angle in y axis.", 0, 360))

function z(val)
  if val then
    z_ = val
  end
  return z_
end
inlet('z', RangeIO("Rotation angle in z axis.", 0, 360))

function opacity(val)
  if val then
    opacity_ = val
  end
  return opacity_
end
inlet('opacity', RangeIO("Change video opacity.", 0, 1))

function blur(val)
  if val then
    blur_ = val
    blur_size = cv.Size(blur_, blur_)
  end
  return blur_
end
inlet('blur', RangeIO("Blur image", 1, 16))

-- TODO: create an 'accessor' function:
-- accessor('z', RangeIO("Rotation angle in z axis.", 0, 360))

function video(frame)
  if not frame then return nil end
  cv.resize(frame, v_small, size, 0, 0, cv.INTER_LINEAR)
  cv.blur(v_small, v_res, blur_size)
  frame_changed = true

  n = n + math.pi / 300
  -- x = 0 --math.cos(n / 0.9) * 360 / math.pi
  -- y = math.sin(n / 0.7) * 360 / math.pi
  -- z = 0 --math.sin(n) * 360 / math.pi
end
inlet('video', MatrixIO('send me images'))

function draw()
  gl.Clear( "COLOR_BUFFER_BIT, DEPTH_BUFFER_BIT")
  --gl.ClearDepth(1.0)
  gl.MatrixMode("MODELVIEW")
  gl.LoadIdentity()

  if frame_changed then
    rk.makeTexture(v_res, tex)
    frame_changed = false
  end

  gl.Translate(0.0, 0.0, -6.0)

  gl.Rotate(x_, 1.0, 0.0, 0.0)
  gl.Rotate(y_, 0.0, 1.0, 0.0)
  gl.Rotate(z_, 0.0, 0.0, 1.0)


  gl.Color(0.5,0.5,0.0)--,0.3)
  gl.LineWidth(1.0)
  glut.WireCube(2.6)

  gl.Color(0.5,0.5,0.0,0.5)
  glut.SolidCube(1.6)

  -- simples way to draw a texture
  gl.Translate(0.0, 0.0, 1.6)
  -- do not forget to set a color or nothing will be drawn
  gl.Color(1,1,1,opacity_)
  rk.drawTexture(tex, 1.3, 1.3, -1.3, -1.3)

  -- simples way to draw a texture
  gl.Translate(0.0, 0.0, -1.6)
  -- do not forget to set a color or nothing will be drawn
  gl.Color(1,1,1,0.5)
 -- rk.drawTexture(tex, 1.3, 1.3, -1.3, -1.3)
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

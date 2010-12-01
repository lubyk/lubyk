require('cv')

-- Load image
img     = img or cv.Mat()

img = cv.LoadImage('../examples/GLLua/rk.png')

-- rebuild texture
img_changed = true

n = n or 0
x = x or 0
y = y or 0
z = z or 0
dt = dt or (math.pi / 1000)

function advance()
  n = n + dt
  x_ = math.cos(n / 0.9) * 360 / math.pi
  y_ = math.sin(n / 0.7) * 360 / math.pi
  z_ = math.sin(n) * 360 / math.pi
end

function draw()
  advance()
  gl.Clear( "COLOR_BUFFER_BIT, DEPTH_BUFFER_BIT")
  --gl.ClearDepth(1.0)
  gl.MatrixMode("MODELVIEW")
  gl.LoadIdentity()

  if img_changed then
    rk.makeTexture(img, tex)
    img_changed = false
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

  gl.PushMatrix()
    -- simples way to draw a texture
    gl.Translate(0.0, 0.0, 1.3)
    -- do not forget to set a color or nothing will be drawn
    gl.Color(1,1,1,0.5)
    rk.drawTexture(tex, 1.3, 1.3, -1.3, -1.3)
  gl.PopMatrix()


  gl.PushMatrix()
    gl.Rotate(90, 0.0, 1.0, 0.0)
    gl.Translate(0.0, 0.0, 1.3)
    rk.drawTexture(tex, 1.3, 1.3, -1.3, -1.3)
  gl.PopMatrix()

end

function resize(width, height)
  print('resize')
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

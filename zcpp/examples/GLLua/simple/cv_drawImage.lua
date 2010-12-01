img_ = img_ or cv.Mat()

function set_image(img)
  print('set_image')
  if img then
    img_changed = true
    img_ = img
  end
  return img_
end
Inlet("set_image", MatrixIO("Image to display."))


function draw()
  gl.Clear( "COLOR_BUFFER_BIT, DEPTH_BUFFER_BIT")
  --gl.ClearDepth(1.0)
  gl.MatrixMode("MODELVIEW")
  gl.LoadIdentity()

  if img_changed then
    rk.makeTexture(img_, tex)
    img_changed = false
  end

  gl.Translate(0.0, 0.0, -7.0)

  gl.PushMatrix()
    -- do not forget to set a color or nothing will be drawn
    gl.Color(1,1,1,1)
    -- draw upside-down because this is how OpenGL sees the textures
    rk.drawTexture(tex, -3, -3, 3, 3, true)
    -- hand made texture coordinates calculations:
    -- draw_image(tex, -3, -3, 3, 3)
  gl.PopMatrix()

end

function draw_image(tex, start_x, start_y, end_x, end_y)
  gl.BindTexture("TEXTURE_2D", tex)
  gl.Enable("TEXTURE_2D")
  gl.Begin("QUADS");
    gl.TexCoord(0.0, 0.0); gl.Vertex(start_x, end_y     )
    gl.TexCoord(1.0, 0.0); gl.Vertex(end_x  , end_y     )
    gl.TexCoord(1.0, 1.0); gl.Vertex(end_x  , start_y   )
    gl.TexCoord(0.0, 1.0); gl.Vertex(start_y, start_y   )
  gl.End();
  gl.Disable("TEXTURE_2D")
end

function draw_rect(x1,y1,x2,y2)
  gl.Begin("LINE_STRIP")
    gl.Vertex(x1,y1,0)
    gl.Vertex(x2,y1,0)
    gl.Vertex(x2,y2,0)
    gl.Vertex(x1,y2,0)
    gl.Vertex(x1,y1,0)
  gl.End()
end

function resize(width, height)
  img_changed = true
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

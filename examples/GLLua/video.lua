require('cv')
res   = res   or cv.Mat()

-- must be a power of 2 !
size = cv.Size(256,256)

function video(frame)
  cv.resize(frame, res, size, 0, 0, cv.INTER_LINEAR)
  cv.flip(res, res, -1)
  frame_changed = true
end
inlet('video', MatrixIO('send me images'))

function draw()
  gl.Clear( "COLOR_BUFFER_BIT, DEPTH_BUFFER_BIT")
  gl.MatrixMode("MODELVIEW")
  gl.LoadIdentity()

  if frame_changed then
    rk.makeTexture(res, tex)
    frame_changed = false
  end

  -- draw texture in rectangle
  gl.Color(1,1,1) -- do not forget !
  rk.drawTexture(tex, 0, 0, view.width, view.height)
end

function resize(width, height)
  textures = textures or gl.GenTextures(1)
  tex = textures[1]

  gl.Enable("TEXTURE_2D")

  gl.MatrixMode("PROJECTION")
  -- reset
  gl.LoadIdentity()
  gl.Ortho(
    0,       -- left
    width,   -- right
    0,       -- bottom
    height,  -- top
    0,       -- near
    1)       -- far
  view.width  = width
  view.height = height
end




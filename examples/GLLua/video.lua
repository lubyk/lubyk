--[[
lib("lib")
w = GLWindow()
l = GLLua("../examples/GLLua/video.lua")
w => l
m = Metro(3000)
m => w
v = VideoIn()
v => video~l
--]]

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

  gl.PushMatrix()
    gl.Enable("TEXTURE_2D")
    gl.Color(1.0,1.0,1.0, 1.0)
    gl.Begin("QUADS")
      gl.BindTexture( "TEXTURE_2D", tex)
      gl.TexCoord(0.0, 0.0);  gl.Vertex(0,0)
      gl.TexCoord(1.0, 0.0);  gl.Vertex(view.width,0)
      gl.TexCoord(1.0, 1.0);  gl.Vertex(view.width, view.height)
      gl.TexCoord(0.0, 1.0);  gl.Vertex(0, view.height)
    gl.End()
    gl.Disable("TEXTURE_2D")
  gl.PopMatrix()
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




--[[  Run with (in build directory)
lib("lib")
win = GLWindow()
cv = GLLua("../examples/GLLua/opencv.lua")
win => cv
video = VideoIn()
video => video~cv
m = Metro(3000)
m => win
--]]

res   = res   or cv.Mat()
small = small or cv.Mat()
blur  = blur  or cv.Mat()
size    = cv.Size(128,128)
blur_k  = cv.Size(10,10)

n = n or 0
x = x or 0
y = y or 0
z = z or 0

function video(frame)
  cv.resize(frame, res, size, 0, 0, cv.INTER_LINEAR)
  frame_changed = true
--  rk.build_mipmaps(res)
  --cv.blur(small, res, blur_k)
  --N = 7
  --lowThresh = 0
  --highThresh = 100
  --aperature_size = N
	--cv.Canny(frame, small, lowThresh*N*N, highThresh*N*N, aperature_size )
  --n = 12.972683264223
  --cv.subtract(cv.Mat(3,3,cv.CV_32FC1), cv.Mat(3,3,cv.CV_32FC2), cv.Mat(3,3,cv.CV_32FC2))
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
    load_texture(tex, res)
    frame_changed = false
  end

  gl.Translate(0.0, 0.0, -6.0)

  gl.Rotate(x, 1.0, 0.0, 0.0)
  gl.Rotate(y, 0.0, 1.0, 0.0)
  gl.Rotate(z, 0.0, 0.0, 1.0)


  gl.Color(0.5,0.5,0.0)--,0.3)
  gl.LineWidth(1.0)
  glut.WireCube(2.6)

  gl.Color(0.5,0.5,0.0, 0.4)
--  gl.Enable("TEXTURE_2D")
--    tex_cube(1.3)
--  gl.Disable("TEXTURE_2D")
  glut.SolidCube(2.6)

  gl.PushMatrix()
    gl.Enable("TEXTURE_2D")
    gl.Translate(0.0, 0.0, 1.9)
    gl.Color(1.0,1.0,1.0, 0.5)
    gl.Begin("QUADS")
      gl.BindTexture( "TEXTURE_2D", tex)
      gl.TexCoord(0.0, 0.0); gl.Vertex(-1.3,-1.3);
      gl.TexCoord(1.0, 0.0); gl.Vertex( 1.3,-1.3);
      gl.TexCoord(1.0, 1.0); gl.Vertex( 1.3, 1.3);
      gl.TexCoord(0.0, 1.0); gl.Vertex(-1.3, 1.3);
    gl.End()
    --rk.draw_matrix(res, 1.3, 1.3, -1.3, -1.3, 0.6)
    gl.Disable("TEXTURE_2D")
  gl.PopMatrix()

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

res_converted = res_converted or cv.Mat()

-- create a matrix filled with green color
-- red = cv.Mat(64, 64, cv.CV_32FC3, cv.Scalar(0.3,0.8,0))

function load_texture(tex, mat)
  cv.cvtColor(mat, res_converted, cv.CV_RGB2BGR)
  rk.makeTexture(mat, tex)
  --[[
  gl.BindTexture( "TEXTURE_2D", tex)
  gl.TexEnv( "TEXTURE_ENV", "TEXTURE_ENV_MODE", "MODULATE" )

  -- when texture area is small, bilinear filter the closest mipmap
  gl.TexParameter( "TEXTURE_2D", "TEXTURE_MIN_FILTER",
                   "LINEAR_MIPMAP_NEAREST" )
  -- when texture area is large, bilinear filter the original
  gl.TexParameter( "TEXTURE_2D", "TEXTURE_MAG_FILTER", "LINEAR" )

  -- the texture wraps over at the edges (repeat)
  gl.TexParameter( "TEXTURE_2D", "TEXTURE_WRAP_S", "REPEAT" )
  gl.TexParameter( "TEXTURE_2D", "TEXTURE_WRAP_T", "REPEAT" )

  --gl.TexImage(mat) -- not working ?
  glu.Build2DMipmaps(mat)
  --]]
end

function tex_cube(sz)
  gl.Begin("QUADS")
		-- Front Face
		gl.TexCoord(0.0, 0.0); gl.Vertex(-sz, -sz,  sz);
		gl.TexCoord(1.0, 0.0); gl.Vertex( sz, -sz,  sz);
		gl.TexCoord(1.0, 1.0); gl.Vertex( sz,  sz,  sz);
		gl.TexCoord(0.0, 1.0); gl.Vertex(-sz,  sz,  sz);
		-- Back Face
		gl.TexCoord(1.0, 0.0); gl.Vertex(-sz, -sz, -sz);
		gl.TexCoord(1.0, 1.0); gl.Vertex(-sz,  sz, -sz);
		gl.TexCoord(0.0, 1.0); gl.Vertex( sz,  sz, -sz);
		gl.TexCoord(0.0, 0.0); gl.Vertex( sz, -sz, -sz);
		-- Top Face
		gl.TexCoord(0.0, 1.0); gl.Vertex(-sz,  sz, -sz);
		gl.TexCoord(0.0, 0.0); gl.Vertex(-sz,  sz,  sz);
		gl.TexCoord(1.0, 0.0); gl.Vertex( sz,  sz,  sz);
		gl.TexCoord(1.0, 1.0); gl.Vertex( sz,  sz, -sz);
		-- Bottom Face
		gl.TexCoord(1.0, 1.0); gl.Vertex(-sz, -sz, -sz);
		gl.TexCoord(0.0, 1.0); gl.Vertex( sz, -sz, -sz);
		gl.TexCoord(0.0, 0.0); gl.Vertex( sz, -sz,  sz);
		gl.TexCoord(1.0, 0.0); gl.Vertex(-sz, -sz,  sz);
		-- Right face
		gl.TexCoord(1.0, 0.0); gl.Vertex( sz, -sz, -sz);
		gl.TexCoord(1.0, 1.0); gl.Vertex( sz,  sz, -sz);
		gl.TexCoord(0.0, 1.0); gl.Vertex( sz,  sz,  sz);
		gl.TexCoord(0.0, 0.0); gl.Vertex( sz, -sz,  sz);
		-- Left Face
		gl.TexCoord(0.0, 0.0); gl.Vertex(-sz, -sz, -sz);
		gl.TexCoord(1.0, 0.0); gl.Vertex(-sz, -sz,  sz);
		gl.TexCoord(1.0, 1.0); gl.Vertex(-sz,  sz,  sz);
		gl.TexCoord(0.0, 1.0); gl.Vertex(-sz,  sz, -sz);
	gl.End()
end


-- function dealloc()
--   gl.DeleteTextures(1, textures)
-- end


-- missing defs from cv.h
cv.CV_RGB2BGR = 4



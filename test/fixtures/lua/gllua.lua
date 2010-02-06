-- # in  build
-- lib("lib")
-- w = GLWindow()
-- l = GLLua("../test/fixtures/lua/gllua.lua")
-- w => l
-- m = Metro(3000)
-- m => l/in/bang
-- I want
-- m => bang.l

x = 200
y = 200
n = 0.0
s = 8
in3  = in3 or {120,210}
rtri = 0

function bang(sig)
  n = n + math.pi / 200
  x = math.cos(n / 0.9) * 360 / math.pi;
  y = math.sin(n) * 360 / math.pi;
end
inlet('bang', NilIO('just bang me'))

function draw(sig)
  gl.Enable("CULL_FACE")
  gl.Translate(0.0,0.0,-5.0)
  gl.Rotate(x, 1.0, 0.0, 0.0)
  gl.Rotate(y, 0.0, 1.0, 0.0)
  gl.Begin("QUADS")
    gl.Color(0.0,0.8,0.0,0.9)
    gl.Vertex( 1.0, 1.0,-1.0)
    gl.Vertex(-1.0, 1.0,-1.0)
    gl.Vertex(-1.0, 1.0, 1.0)
    gl.Vertex( 1.0, 1.0, 1.0)
    gl.Color(0.8,0.5,0.0,0.9)
    gl.Vertex( 1.0,-1.0, 1.0)
    gl.Vertex(-1.0,-1.0, 1.0)
    gl.Vertex(-1.0,-1.0,-1.0)
    gl.Vertex( 1.0,-1.0,-1.0)
    gl.Color(0.8,0.0,0.0,0.9)
    gl.Vertex( 1.0, 1.0, 1.0)
    gl.Vertex(-1.0, 1.0, 1.0)
    gl.Vertex(-1.0,-1.0, 1.0)
    gl.Vertex( 1.0,-1.0, 1.0)
    gl.Color(0.8,0.8,0.0,0.9)
    gl.Vertex( 1.0,-1.0,-1.0)
    gl.Vertex(-1.0,-1.0,-1.0)
    gl.Vertex(-1.0, 1.0,-1.0)
    gl.Vertex( 1.0, 1.0,-1.0)
    gl.Color(0.0,0.0,0.8,0.9)
    gl.Vertex(-1.0, 1.0, 1.0)
    gl.Vertex(-1.0, 1.0,-1.0)
    gl.Vertex(-1.0,-1.0,-1.0)
    gl.Vertex(-1.0,-1.0, 1.0)
    gl.Color(0.8,0.0,0.8,0.9)
    gl.Vertex( 1.0, 1.0,-1.0)
    gl.Vertex( 1.0, 1.0, 1.0)
    gl.Vertex( 1.0,-1.0, 1.0)
    gl.Vertex( 1.0,-1.0,-1.0)
  gl.End()

end
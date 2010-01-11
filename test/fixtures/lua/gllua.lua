x = 200
y = 200
n = 0.0
s = 8
in3  = in3 or {120,210}
rtri = 0

function bang(sig)
  n = n + math.pi / 90
  x = in3[1] + math.cos(n) * 10
  y = in3[2] + math.sin(n) * 20
end

function draw(sig)
	gl.Enable("CULL_FACE")
  gl.MatrixMode('PROJECTION')
  gl.PushMatrix()
  gl.LoadIdentity()
  gl.Perspective(45.0, sig[1] / sig[2], 0.1, 100.0)
  gl.MatrixMode("MODELVIEW")
  gl.PushMatrix()
  gl.LoadIdentity()
  gl.ClearDepth(1)
  
  gl.Translate(-1.5,0.0,-6.0)
  gl.Rotate(in3[1] / 2,0,1.0,0.0)
  gl.Rotate(in3[2] / 2,1.0,0,0.0)
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
  
  gl.PopMatrix()
  gl.MatrixMode("PROJECTION")
  gl.PopMatrix()
  
  gl.Translate(x,y,  0)
  gl.Color( {1, 1, 0, 0.8} )
  gl.Rect(-s,-s,s,s)
end
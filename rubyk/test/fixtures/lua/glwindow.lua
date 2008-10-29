x = 200
y = 200
n = n or 0.0
s = 8
mouse  = mouse or {409,223}
rtri = 0

function init()
  gl.Disable("BLEND")
  gl.Enable("DEPTH_TEST")
  gl.Enable("CULL_FACE")
	gl.ClearDepth(1.0)
  gl.DepthFunc("LEQUAL")
  gl.ClearColor(0.2,0.2,0.2,0.5)
end

function mouse_move(sig)
	if (sig[4] == 0) then
		-- mouseDown
		mouse[1] = sig[1]
		mouse[2] = sig[2]
	end
end

function draw(sig)
	n = current_time / 500
	x = mouse[1] + math.cos(n * 1.2) * 10
	y = mouse[2] + math.sin(n) * 20
  gl.MatrixMode('PROJECTION')
  gl.LoadIdentity()
  gl.Perspective(45.0, sig[1] / sig[2], 0.1, 100.0)
  gl.MatrixMode("MODELVIEW")
  gl.PushMatrix()
  gl.LoadIdentity()
  gl.ClearDepth(1)
  
  gl.Translate(-1.5,0.0,-6.0)
  gl.Rotate(x,0,1.0,0.0)
  gl.Rotate(y,1.0,0.0,0.0)
  gl.Rotate(mouse[2] / 2,1.0,0,0.0)
  gl.Begin("QUADS")
    gl.Color(0.0,0.8,0.0,1)
    gl.Vertex( 1.0, 1.0,-1.0)
    gl.Vertex(-1.0, 1.0,-1.0)
    gl.Color(0.8,0.0,0.0,1)
    gl.Vertex(-1.0, 1.0, 1.0)
    gl.Vertex( 1.0, 1.0, 1.0)

    gl.Color(0.8,0.0,0.0,1)
    gl.Vertex( 1.0,-1.0, 1.0)
    gl.Vertex(-1.0,-1.0, 1.0)
    gl.Color(0.8,0.5,0.0,1)
    gl.Vertex(-1.0,-1.0,-1.0)
    gl.Vertex( 1.0,-1.0,-1.0)

    gl.Color(0.6,0.0,0.0,1)
    gl.Vertex( 1.0, 1.0, 1.0)
    gl.Vertex(-1.0, 1.0, 1.0)
    gl.Vertex(-1.0,-1.0, 1.0)
    gl.Vertex( 1.0,-1.0, 1.0)

    gl.Color(0.0,0.0,0.0,1)
    gl.Vertex( 1.0,-1.0,-1.0)
    gl.Vertex(-1.0,-1.0,-1.0)
    gl.Vertex(-1.0, 1.0,-1.0)
    gl.Vertex( 1.0, 1.0,-1.0)

    gl.Color(0.8,0.0,0.0,1)
    gl.Vertex(-1.0, 1.0, 1.0)
    gl.Color(0.0,0.0,0.8,1)
    gl.Vertex(-1.0, 1.0,-1.0)
    gl.Vertex(-1.0,-1.0,-1.0)
    gl.Color(0.8,0.0,0.0,1)
    gl.Vertex(-1.0,-1.0, 1.0)

    gl.Color(0.8,0.0,0.8,1)
    gl.Vertex( 1.0, 1.0,-1.0)
    gl.Color(0.8,0.0,0.0,1)
    gl.Vertex( 1.0, 1.0, 1.0)
    gl.Vertex( 1.0,-1.0, 1.0)
    gl.Color(0.8,0.0,0.8,1)
    gl.Vertex( 1.0,-1.0,-1.0)
  gl.End()
  
	draw_looper()
	
  gl.PopMatrix()
	
  gl.Viewport(0, 0, sig[1], sig[2])
  gl.MatrixMode("PROJECTION")
  gl.LoadIdentity()
  gl.Ortho2D(0, sig[1], 0, sig[2])
  gl.MatrixMode("MODELVIEW")
  gl.LoadIdentity()

  
  gl.Translate(mouse[1] + math.cos(n*6)*15,mouse[2] + math.sin(n*6)*15,  0)
  gl.Color( {1, 1, 0, 0.8} )
  gl.Rect(-s,-s,s,s)
end

function draw_looper()
	local ls = 0.1
	local lx = math.cos(n/3) * 0.8
	gl.PushMatrix()
		gl.Translate(0,0,1)
		gl.Translate(lx,0,0)
	  gl.Color(1, 1, 0, 0.8)
	  gl.Rect(-ls,-ls,ls,ls)
	gl.PopMatrix()
end
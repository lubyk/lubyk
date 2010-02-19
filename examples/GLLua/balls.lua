-- # in  build
--[[
lib("lib")
w = GLWindow()
l = GLLua("../examples/GLLua/balls.lua")
w => l
m = Metro(3000)
m => bang~l
--]]

count = 100
alpha = 0.4
step  = 0.05
blend = true
mono  = false
light = false
const_att = 0.7

function make_dots()
  math.randomseed( os.time() )
  local dots = {}
  for i = 1,count do
    dots[i] = {
      x=0.5 - math.random(),
      y=0.5 - math.random(),
      z=0.5 - math.random(),
      vx=0.5 - math.random(),
      vy=0.5 - math.random(),
      vz=0.5 - math.random(),
      sz=0.3 * math.random(),
      r=math.random(),
      v=math.random(),
      b=math.random()
    }
  end
  return dots
end

dots = make_dots()

dimx = {min=-1.3, max=1.3}
dimy = {min=-1.3, max=1.3}
dimz = {min=-1.3, max=1.3}

white = { 1.0, 1.0, 1.0, 1.0 }
black = { 0.0, 0.0, 0.0, 1.0 }

n = n or 0
x = x or 0
y = y or 0
z = z or 0

function bang(sig)
  for k,dot in pairs(dots) do
    move(dot)
  end
  n = n + math.pi / 2000
  x = math.cos(n / 0.9) * 360 / math.pi
  y = math.sin(n / 0.7) * 360 / math.pi
  z = math.sin(n) * 360 / math.pi
end
inlet('bang', NilIO('just bang me'))

function draw(sig)
  gl.Enable("POINT_SMOOTH")
  gl.Enable("LINE_SMOOTH")
  if blend then
    gl.Enable("BLEND")
    gl.Disable("DEPTH_TEST")
  else
    gl.Disable("BLEND")
    gl.Enable("DEPTH_TEST")
  end
  gl.BlendFunc("SRC_ALPHA", "ONE_MINUS_SRC_ALPHA")
  gl.Disable("LIGHTING")
  gl.Disable("CULL_FACE")
  if light then set_light() end

  gl.Translate(0.0, 0.0, -6.0)
  if light then light_on() end

  gl.Rotate(x, 1.0, 0.0, 0.0)
  gl.Rotate(y, 0.0, 1.0, 0.0)
  gl.Rotate(z, 0.0, 0.0, 1.0)
  gl.Color(0.8,0.8,0.0,0.3)
  glut.WireCube(2.6)
  gl.Color(0.8,0.8,0.0,0.1)
  --glut.SolidCube(2.6)
  --draw_cube(2.0, 0.5)
  --gl.Begin("TRIANGLE_STRIP")
    for i = 1,count do
      dot = dots[i]
      dot_color(dot)
      gl.PushMatrix()
        gl.Translate(dot.x, dot.y, dot.z)
        glut.SolidSphere(dot.sz, dot.sz * 100, dot.sz * 100)
      gl.PopMatrix()
      --gl.Vertex(dot.x, dot.y, dot.z)
    end
  --gl.End()

  gl.PointSize(5.0)
  --gl.Begin("POINTS")
  --  for k,dot in pairs(dots) do
  --    dot_color(dot)
  --    gl.Vertex(dot.x, dot.y, dot.z)
  --  end
  --gl.End()

  gl.Color(0.5,1.0,0.5)
  --draw_rect(dimx.min, dimy.min, dimx.max, dimy.max)
end

function move(dot)
  dot.x = dot.x + dot.vx * step
  if (dot.x > dimx.max) then
    dot.x = dimx.max
    dot.vx = -dot.vx
  elseif (dot.x < dimx.min) then
    dot.x = dimx.min
    dot.vx = -dot.vx
  end

  dot.y = dot.y + dot.vy * step
  if (dot.y > dimy.max) then
    dot.y = dimy.max
    dot.vy = -dot.vy
  elseif (dot.y < dimy.min) then
    dot.y = dimy.min
    dot.vy = -dot.vy
  end

  dot.z = dot.z + dot.vz * step / 3
  if (dot.z > dimz.max) then
    dot.z = dimz.max
    dot.vz = -dot.vz
  elseif (dot.z < dimz.min) then
    dot.z = dimz.min
    dot.vz = -dot.vz
  end
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

function dot_color(dot)
  far = ( 2 * math.abs(dot.z)) / (dimz.max - dimz.min)
  --gl.Color(far * dot.r, far * dot.v, far * dot.b, alpha)
  --gl.Color(dot.r, dot.v, dot.b, alpha)
  if mono then
    gl.Color(0.5,0.5,0.2,alpha)
  elseif light then
  	gl.Material("FRONT", "AMBIENT_AND_DIFFUSE", {dot.r, dot.v, dot.b, 1.0})
  	gl.Material("FRONT", "SPECULAR", black)
  	gl.Material("FRONT", "SHININESS", {100.0})
	else
    gl.Color(dot.r, dot.v, dot.b, alpha)
  end
end


function draw_cube(side, a)
  l = side / 2
  gl.Begin("QUADS")
    gl.Color(0.0,0.8,0.0,a)
    gl.Vertex( l, l,-l)
    gl.Vertex(-l, l,-l)
    gl.Vertex(-l, l, l)
    gl.Vertex( l, l, l)

    gl.Color(0.8,0.5,0.0,a)
    gl.Vertex( l,-l, l)
    gl.Vertex(-l,-l, l)
    gl.Vertex(-l,-l,-l)
    gl.Vertex( l,-l,-l)

    gl.Color(0.8,0.0,0.0,a)
    gl.Vertex( l, l, l)
    gl.Vertex(-l, l, l)
    gl.Vertex(-l,-l, l)
    gl.Vertex( l,-l, l)

    gl.Color(0.8,0.8,0.0,a)
    gl.Vertex( l,-l,-l)
    gl.Vertex(-l,-l,-l)
    gl.Vertex(-l, l,-l)
    gl.Vertex( l, l,-l)

    gl.Color(0.0,0.0,0.8,a)
    gl.Vertex(-l, l, l)
    gl.Vertex(-l, l,-l)
    gl.Vertex(-l,-l,-l)
    gl.Vertex(-l,-l, l)

    gl.Color(0.8,0.0,0.8,a)
    gl.Vertex( l, l,-l)
    gl.Vertex( l, l, l)
    gl.Vertex( l,-l, l)
    gl.Vertex( l,-l,-l)
  gl.End()

end

function set_light()

  gl.Enable("LIGHTING")

	gl.Enable("LIGHT0")
	gl.Light("LIGHT0", "DIFFUSE", white)
	gl.Light("LIGHT0", "SPECULAR", white)
end

function light_on()
  -- Position the light and show where it is
	gl.PushMatrix()
		gl.Translate(3, 3, 3)
		gl.Light("LIGHT0", "CONSTANT_ATTENUATION", const_att)
		gl.Light("LIGHT0", "POSITION", {0,0,0})
		gl.Disable("LIGHTING")
		gl.Color(0.9, 0.9, 0.5)
		glut.SolidSphere(0.1, 10, 10)
		gl.Enable("LIGHTING")
	gl.PopMatrix()
end
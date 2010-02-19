-- # in  build
--[[
lib("lib")
w = GLWindow()
l = GLLua("../examples/GLLua/poly_blob.lua")
w => l
m = Metro(3000)
m => bang~l
--]]


function make_dots(count)
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
      r=math.random(),
      v=math.random(),
      b=math.random()
    }
  end
  return dots
end

dots = make_dots(1005)

dimx = {min=-1.3, max=1.3}
dimy = {min=-1.3, max=1.3}
dimz = {min=-1.3, max=1.3}

step = 0.05

n = n or 0
x = 0
y = 0
z = 0

function bang(sig)
  for k,dot in pairs(dots) do
    move(dot)
  end
  --n = n + math.pi / 200
  x = math.cos(n / 0.9) * 360 / math.pi
  y = math.sin(n / 0.7) * 360 / math.pi
  z = math.sin(n) * 360 / math.pi
end
inlet('bang', NilIO('just bang me'))

function draw(sig)
  gl.Enable("POINT_SMOOTH")
  gl.Enable("LINE_SMOOTH")
  gl.Enable("BLEND")
  gl.Enable("DEPTH_TEST")
  gl.Disable("LIGHTING")

  gl.Translate(0.0, 0.0, -6.0)
  gl.Rotate(x, 1.0, 0.0, 0.0)
  gl.Rotate(y, 0.0, 1.0, 0.0)
  gl.Rotate(z, 0.0, 0.0, 1.0)
  gl.Begin("POLYGON")
    for k,dot in pairs(dots) do
      dot_color(dot)
      gl.Vertex(dot.x, dot.y, dot.z)
    end
  gl.End()

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
  gl.Color(far * dot.r, far * dot.v, far * dot.b)
end


-- # in  build
--[[
lib("lib")
w = GLWindow()
l = GLLua("../examples/GLLua/splines.lua")
w => l
m = Metro(3000)
m => advance~l
--]]

count = 9
alpha = 0.3
step  = 0.01
blend = true
mono  = false
light = false
const_att = 0.7

splines = true
resolution = 50
rebuild = true
old_count = old_count or 0

function make_dots(nb)
  math.randomseed( os.time() )
  local next = nil
  local j   = 1
  local dot = nil
  local dots = {}
  for i = nb,1,-1 do
    dot = {
      next = next,
      pos  = {
        0 * math.random(), -- x
        0 * math.random(), -- y
        0 * math.random()  -- z
      },
      ctrl = {
        0 * math.random(), -- x
        0 * math.random(), -- y
        0 * math.random()  -- z
      },
      move = {
        0.5 - math.random(), -- vx
        0.5 - math.random(), -- vy
        0.5 - math.random()  -- vz
      },
      cmove = {
        0.5 - math.random(), -- vx
        0.5 - math.random(), -- vy
        0.5 - math.random()  -- vz
      },
      color = {
        r = math.random(),
        v = math.random(),
        b = math.random()
      },
      ctrls = {0,0,0,0}

    }
    dots[i] = dot
    next = dot
  end
  return dots
end

function make_ctrl_points(rebuild_all)
  local dot = nil
  if true then
    for i = 1,(old_count-1) do
      local ctrls = {}
      dot = dots[i]
      -- (1)
      ctrls[1] = dot.pos
      -- (2)
      ctrls[2] = {
        dot.pos[1] + dot.ctrl[1],
        dot.pos[2] + dot.ctrl[2],
        dot.pos[3] + dot.ctrl[3]
      }
      -- (3)
      ctrls[3] = {
        dot.next.pos[1] - dot.next.ctrl[1],
        dot.next.pos[2] - dot.next.ctrl[2],
        dot.next.pos[3] - dot.next.ctrl[3]
      }
      -- (4)
      ctrls[4]  = dot.next.pos
      dot.ctrls = ctrls
    end
  else
    for i = 1,(old_count-1) do
      local ctrls = {}
      dot = dots[i]
      ctrls = dot.ctrls
      -- (1)
      -- (2)
      ctrls[2] = {
        dot.pos[1] + dot.ctrl[1],
        dot.pos[2] + dot.ctrl[2],
        dot.pos[3] + dot.ctrl[3]
      }
      -- (3)
      ctrls[3] = {
        dot.next.pos[1] - dot.next.ctrl[1],
        dot.next.pos[2] - dot.next.ctrl[2],
        dot.next.pos[3] - dot.next.ctrl[3]
      }
      -- (4)
    end
  end
end

dots = dots or {}

if count ~= old_count then
  dots = make_dots(count)
  old_count = count
  make_ctrl_points(true)
end

dim = {
  {min=-1.3, max=1.3}, -- x
  {min=-1.3, max=1.3}, -- y
  {min=-1.3, max=1.3}  -- z
}

white = { 1.0, 1.0, 1.0, 1.0 }
black = { 0.0, 0.0, 0.0, 1.0 }

n = n or 0
x = x or 0
y = y or 0
z = z or 0

view = view or {
  w = 0,
  h = 0
}

function advance()
  for k,dot in pairs(dots) do
    move(dot.pos, dot.move)
    move(dot.ctrl, dot.cmove)
  end

  make_ctrl_points(false)

  n = n + math.pi / 1000
  x = math.cos(n / 0.9) * 360 / math.pi
  y = math.sin(n / 0.7) * 360 / math.pi
  z = math.sin(n) * 360 / math.pi
end
inlet('advance', NilIO('just bang me'))

function draw()
  if blend then
    gl.Enable("BLEND")
    gl.Disable("DEPTH_TEST")
  else
    gl.Disable("BLEND")
    gl.Enable("DEPTH_TEST")
  end
  gl.BlendFunc("SRC_ALPHA", "ONE_MINUS_SRC_ALPHA")
  gl.Disable("CULL_FACE")

  --gl.Clear( "COLOR_BUFFER_BIT, DEPTH_BUFFER_BIT")

  gl.MatrixMode("MODELVIEW")
  gl.LoadIdentity()

  gl.Color(0.0,0,0,0.03)
  gl.Begin("QUADS")
    gl.Vertex(-view.width/2, -view.height/2, -6.0)
    gl.Vertex( view.width/2, -view.height/2, -6.0)
    gl.Vertex( view.width/2,  view.height/2, -6.0)
    gl.Vertex(-view.width/2,  view.height/2, -6.0)
  gl.End()

  gl.Translate(0.0, 0.0, -4.0)

  gl.Rotate(x, 1.0, 0.0, 0.0)
  gl.Rotate(y, 0.0, 1.0, 0.0)
  gl.Rotate(z, 0.0, 0.0, 1.0)
  gl.Color(0.5,0.5,0.0,0.3)
  gl.LineWidth(1.0)
  --glut.WireCube(2.6)
  gl.Color(0.5,0.5,0.0,0.1)
  --glut.SolidCube(2.6)

  --gl.Translate(-1.0, -1.2, 0.0)
  draw_curve()
  --draw_handles()
  --draw_ctrl_lines()

  --gl.Begin("POINTS")
  --  for k,dot in pairs(dots) do
  --    dot_color(dot)
  --    gl.Vertex(dot[1], dot[2], dot[3])
  --  end
  --gl.End()

  gl.Color(0.5,1.0,0.5)
end

function move(pos, mv)
  for i = 1,3 do
    pos[i] = pos[i] + mv[i] * step
    if (pos[i] > dim[i].max) then
      mv[i] = -mv[i]
    elseif (pos[i] < dim[i].min) then
      mv[i] = -mv[i]
    end
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
  if mono then
    gl.Color(0.5,0.5,0.2,alpha)
  elseif light then
  	gl.Material("FRONT", "AMBIENT_AND_DIFFUSE", {dot.r, dot.v, dot.b, 1.0})
  	gl.Material("FRONT", "SPECULAR", black)
  	gl.Material("FRONT", "SHININESS", {100.0})
	else
    gl.Color(dot.color.r, dot.color.v, dot.color.b, alpha)
  end
end

function draw_curve()
  gl.PushMatrix()
  local x = 0
  local y = 0
  local z = 0

  if splines then
    gl.LineWidth(3.0)
    gl.Begin("LINE_STRIP")
      for i = 1,(old_count-1) do
        dot = dots[i]
        dot_color(dot)
        ctrls = dot.ctrls
        p1 = ctrls[1]
        p2 = ctrls[2]
        p3 = ctrls[3]
        p4 = ctrls[4]
        c1 = dot.color
        c4 = dot.next.color
        for t = 0,1.0,(1.0/resolution) do
          ta = (1-t)^3
          tb = 3 * t * (1-t)^2
          tc = 3 * (1-t) * t^2
          td = t^3

          gl.Color(
            c1.r * (1-t) + c4.r * t,
            c1.v * (1-t) + c4.v * t,
            c1.b * (1-t) + c4.b * t
          )
          gl.Vertex(
            ta * p1[1] + tb * p2[1] + tc * p3[1] + td * p4[1],
            ta * p1[2] + tb * p2[2] + tc * p3[2] + td * p4[2],
            ta * p1[3] + tb * p2[3] + tc * p3[3] + td * p4[3]
          )
        end

        gl.Vertex(p4)
      end
    gl.End()


    gl.PointSize(5.0)
    gl.Begin("POINTS")
      for i = 1,old_count do
        dot = dots[i]
        dot_color(dot)
        gl.Vertex(dot.pos)
      end
    gl.End()
  else
    gl.Begin("LINE_STRIP")
      for i = 1,old_count do
        dot = dots[i]
        dot_color(dot)
        gl.Vertex(dot.pos)
      end
    gl.End()

    gl.PointSize(5.0)
    gl.Begin("POINTS")
      for i = 1,old_count do
        dot = dots[i]
        dot_color(dot)
        gl.Vertex(dot.pos)
      end
    gl.End()
  end
  gl.PopMatrix()
end

function draw_handles()
  gl.PushMatrix()
  gl.LineWidth(0.5)
  gl.Color(0.9,0.5,0.5,1.0)
  for i = 1,old_count do
    dot = dots[i]
    gl.Begin("LINE_STRIP")
      gl.Vertex(dot.pos)
      gl.Vertex(
        dot.pos[1] + dot.ctrl[1],
        dot.pos[2] + dot.ctrl[2],
        dot.pos[3] + dot.ctrl[3]
      )
    gl.End()
  end
  gl.End()

  gl.PointSize(3.0)
  gl.Begin("POINTS")
    for i = 1,old_count do
      dot = dots[i]
      gl.Vertex(
        dot.pos[1] + dot.ctrl[1],
        dot.pos[2] + dot.ctrl[2],
        dot.pos[3] + dot.ctrl[3]
      )
    end
  gl.End()
  gl.PopMatrix()
end


function draw_ctrl_lines()
  gl.PushMatrix()
  gl.Color(0.8,0.0,0.0,0.4)
  gl.Begin("LINE_STRIP")
    for i = 1,(old_count-1) do
      ctrls = dots[i].ctrls
      gl.Vertex(ctrls[1])
      gl.Vertex(ctrls[2])
      gl.Vertex(ctrls[3])
      gl.Vertex(ctrls[4])
    end
  gl.End()
  gl.PopMatrix()
end

function resize(height, width)
  gl.Enable("POINT_SMOOTH")
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
  --gl.Ortho2D(0, width, 0, height)
  -- Select the modelview matrix
  gl.Clear( "COLOR_BUFFER_BIT, DEPTH_BUFFER_BIT")
  view.width  = width
  view.height = height

end
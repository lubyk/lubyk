-- # in  build
--[[
lib("lib")
w = GLWindow()
l = GLLua("../examples/GLLua/lights.lua")
w => l
m = Metro(3000)
m => bang~l
--]]

red = { 0.8, 0.0, 0.0, 1.0 }
blue = { 0.0, 0.2, 1.0, 1.0 }
white = { 1.0, 1.0, 1.0, 1.0 }
black = { 0.0, 0.0, 0.0, 1.0 }
polished = { 100.0 }
dull = { 0.0 }


-- Position the light at the origin.
light_pos = { 10.0, 0.0, 0.0, 1.0 }

-- Translation values for light.
x = 0.0
y = 0.0
z = 0.0


-- Attenuation factors for light.
const_att = 0.8

n = n or 0

function bang(sig)
  n = n + math.pi / 200
  x = 20 * math.cos(n) / math.pi
  y = 10 * math.sin(n) / math.pi
  z = 2 --10 * math.sin(n) / math.pi
end
inlet('bang', BangIO('just bang me'))

function draw(sig)
  gl.PointSize(5.0)
  gl.Enable("CULL_FACE")
  gl.Enable("POINT_SMOOTH")
  gl.Enable("LINE_SMOOTH")
  gl.Enable("BLEND")
  gl.Enable("DEPTH_TEST")
  gl.Enable("LIGHTING")

	gl.Enable("LIGHT0")
	gl.Light("LIGHT0", "DIFFUSE", white)
	gl.Light("LIGHT0", "SPECULAR", white)

	-- Move model into view
	gl.Translate(0.0, 0.0, -30.0)
	gl.Rotate(15, 0, 1, 0)

	-- Position the light and show where it is
	gl.PushMatrix()
		gl.Translate(x, y, z)
		gl.Light("LIGHT0", "CONSTANT_ATTENUATION", const_att)
		gl.Light("LIGHT0", "POSITION", {0,0,0})
		gl.Disable("LIGHTING")
		gl.Color(0.9, 0.9, 0.5)
		glut.SolidSphere(0.1, 10, 10)
		gl.Enable("LIGHTING")
	gl.PopMatrix()

	-- Display a dull, red sphere on the left
	gl.PushMatrix()
  	gl.Translate(-5.0, 0.0, 0.0)
  	gl.Material("FRONT", "AMBIENT_AND_DIFFUSE", red)
  	gl.Material("FRONT", "SPECULAR", black)
  	gl.Material("FRONT", "SHININESS", dull)
  	glut.SolidSphere(3.0, 40, 40)
  gl.PopMatrix()

	-- Display a bright, blue sphere on the right
	gl.PushMatrix()
  	gl.Translate(5.0, 0.0, 0.0)
  	gl.Material("FRONT", "AMBIENT_AND_DIFFUSE", blue)
  	gl.Material("FRONT", "SPECULAR", white)
  	gl.Material("FRONT", "SHININESS", polished)
  	glut.SolidSphere(3.0, 40, 40)
  gl.PopMatrix()
end

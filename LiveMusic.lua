--[[------------------------------------------------------

  # Live movie transformation

  In this tutorial, we transform the frames of a movie.

  Note that you must run this example with `luajit` since plain lua is not
  supported by four.

  ## Download source

  [LiveDecoder.lua](example/lui/LiveDecoder.lua)

--]]------------------------------------------------------
-- doc:lit

-- # Require
--
-- Every script must start by requiring needed libraries. `lui` is the simple
-- lua gui, `four` is the opengl library and `lens` is the scheduling library.
local lens  = require 'lens'
local lui   = require 'lui'
local four  = require 'four'
local video = require 'video'

-- Autoload this script.
lens.run(function() lens.FileWatch() end)

-- Declare some constants.
local WIN_SIZE   = {w = 400, h = 400}
local WIN_POS    = {x = 10 , y = 10 }
local SATURATION = 0.4

-- # Geometry
--
-- We must now prepare the geometry that we want to display. In this example,
-- we simply create two triangles that fill the clip space (= screen).
local function square()
  -- Vertex buffer (list of coordinates).
  local vb = four.Buffer { dim = 3,
             scalar_type = four.Buffer.FLOAT } 
  -- Color buffer (colors for each coordinate).
  local cb = four.Buffer { dim = 4,
             scalar_type = four.Buffer.FLOAT }
  -- Index buffer (actual triangles defined with vertices in `vb`).
  local ib = four.Buffer { dim = 1,
             scalar_type = four.Buffer.UNSIGNED_INT }

  local tex = four.Buffer { dim = 2,
             scalar_type = four.Buffer.FLOAT }
  -- The drawing shows the coordinates and index values that we will use
  -- when filling the index.
  --
  --   #txt ascii
  --   (-1, 1)              (1, 1)
  --     +--------------------+
  --     | 2                3 |
  --     |                    |
  --     |                    |
  --     | 0                1 |
  --     +--------------------+
  --   (-1,-1)              (1, -1)
  --
  -- Create four vertices, one for each corner.
  vb:push3D(-1.0, -1.0, 0.0)
  tex:push2D(1.0, 1.0)

  vb:push3D( 1.0, -1.0, 0.0)
  tex:push2D(0.0, 1.0)

  vb:push3D(-1.0,  1.0, 0.0)
  tex:push2D(1.0, 0.0)

  vb:push3D( 1.0,  1.0, 0.0)
  tex:push2D(0.0, 0.0)

  -- Colors for the positions above.
  cb:pushV4(four.Color.red())
  cb:pushV4(four.Color.green())
  cb:pushV4(four.Color.blue())
  cb:pushV4(four.Color.white())

  -- Create two triangles made of 3 vertices. Note that the index is
  -- zero based.
  ib:push3D(0, 1, 2)
  ib:push3D(1, 3, 2)

  -- Create the actual geometry object with four.Geometry. Set the primitive
  -- to triangles and set index and data with the buffered we just prepared.
  return four.Geometry {
    primitive = four.Geometry.TRIANGLE, 
    index = ib,
    data = { vertex = vb, color = cb, tex = tex}
  }

  -- End of the local `square()` function definition.
end

movie = movie or video.Decoder('plants.MOV')

local V3, Texture = four.V3, four.Texture
function texture(val)
  local data = four.Buffer {
    scalar_type = four.Buffer.UNSIGNED_BYTE,
    cdata = function() return movie:frameData() end,
    csize = function() return movie:frameSize() end,
  }
  
  return Texture { type = Texture.TYPE_2D, 
                   internal_format = Texture.BGRA_8UN,  --RGB_8UN,
                   size = V3(1280, 720, 1),
                   wrap_s = Texture.WRAP_CLAMP_TO_EDGE,
                   wrap_t = Texture.WRAP_CLAMP_TO_EDGE,
                   mag_filter = Texture.MAG_LINEAR,
                   min_filter = Texture.MIN_LINEAR_MIPMAP_LINEAR,
                   generate_mipmaps = true, 
                   data = data }
end


-- # Renderer
--
-- Create the renderer with four.Renderer.
renderer = renderer or four.Renderer {
  size = four.V2(WIN_SIZE.w, WIN_SIZE.h),
}

-- # Camera
--
-- Use four.Camera to create a simple camera.
camera = camera or four.Camera()

-- # Effect
--
-- We create a new four.Effect that will process our geometry and make
-- something nice out of it.
--
-- `default_uniforms` declares the uniforms and sets default values in case
-- the renderable (in our case `obj`) does not redefine these values.
--
-- The special value [RENDER_FRAME_START_TIME](four.Effect.html#RENDER_FRAME_START_TIME) set for `time` will
-- give use the current time in seconds (0 = script start).
effect = effect or four.Effect {}

effect.default_uniforms = {
  saturation = 0.5,
  time = four.Effect.RENDER_FRAME_START_TIME,
}

-- Define the vertex shader. This shader simply passes values along to the
-- fragment shader.
effect.vertex = four.Effect.Shader [[
  in vec4 vertex;
  in vec4 color;
  out vec4 v_vertex;
  out vec4 v_color;
  in  vec2 tex;
  out vec2 v_tex;
  void main()
  {
    v_tex = tex;
    v_vertex = vertex;
    v_color  = color;
    gl_Position = vertex;
  }
]]
  
-- Define the fragment shader. This shader simply creates periodic colors
-- based on pixel position and time.
effect.fragment = four.Effect.Shader [[
  in vec2 v_tex;
  uniform sampler2D movtex;
  in vec4 v_vertex;
  in vec4 v_color;

  // These uniform names must reflect the default_uniforms that we declared.
  uniform float saturation;
  uniform float time;
  float t = time;
  float sat = saturation * 0.8 * (0.5 + 0.5 * sin(t/10));

  out vec4 color;

  void main() {
    vec4 img = texture(movtex, v_tex);
    color = img;
  }
]]

-- # Renderable
--
-- Create a simple renderable object. In four, a renderable is a table that
-- contains enough information to be rendered.
--
-- The @saturation@ parameter is a uniform declaration. The value of this
-- variable will be accessible in the shader (see #Effect above).
-- 
-- We set the geometry to the fullscreen square by calling our function and
-- assign our simple effect.
obj = obj or {
  saturation = SATURATION,
  geometry   = square(),
  effect     = effect,
  movtex     = texture(),
}

local last1 = lens.elapsed()
function movie:newFrame()
  local now = lens.elapsed()
  print(math.floor((now - last1) * 1000 + 0.5))
  --print(1/(now - last1))
  last1 = now
  --print(self:frameInfo())
  obj.movtex = texture()
  --win:draw()
end

tim = lens.Timer(1/10, function()
  if movie:nextFrame() then
    print('yes')
  else
    print('no')
  end
end)
tim:start()

-- # Window
--
-- We create an OpenGL window with lui.View, set the size and position.
if not win then
  win = lui.View()
  win:resize(WIN_SIZE.w, WIN_SIZE.h)
  win:move(WIN_POS.x, WIN_POS.y)
end

-- We then setup some simple keyboard actions to toggle fullscreen with
-- the space bar.
--
-- Only react to key press (press = true).
function win:keyboard(key, press)
  if key == mimas.Key_Space and press then
    win:swapFullscreen()
  end
end

function win:mouseDown()
  self:swapFullscreen()
end


-- In case we resize the window, we want our content to scale so we need to
-- update the renderer's `size` attribute.
function win:resized(w, h)
  renderer.size = four.V2(w, h)
--  self:draw()
end

-- The window's draw function calls four.Renderer.render with our camera
-- and object and then swaps OpenGL buffers.
function win:draw()
  renderer:render(camera, {obj})
  self:swapBuffers()
end


-- Show the window once all the the callbacks are in place.
win:show()

renderer:logInfo()

-- # Runtime

-- ## Timer
-- Since our effect is a function of time, we update at 60 Hz. For this we
-- create a timer to update window content every few milliseconds.
--
-- We also change the uniform's saturation with a random value between
-- [0, 0.8] for a stupid blink effect.
timer = timer or lens.Timer(200)

local last2 = lens.elapsed()
function timer:timeout()
  local now = lens.elapsed()
  --print(math.floor((now - last2) * 1000 + 0.5))
  --print(1/(now - last2))
  last2 = now
  win:draw()
  obj.saturation = math.random() * 0.8
end
timer:setInterval(1/60)

-- Start the timer.
if not timer:running() then
  timer:start(1)
end


--[[
  ## Download source

  [LiveShaderCoding.lua](example/lui/LiveShader.lua)
--]]

--halt_timer = lens.Thread(function()
--  lens.sleep(8)
--  lens.halt()
--end)


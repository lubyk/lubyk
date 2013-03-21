--[[------------------------------------------------------

  # Simple fullscreen effect

  In this tutorial, we create a simple animation with
  [mimas](mimas.html) and [four](four.html), showing how to connect all
  the pieces together from window declaration and callbacks to timers,
  geometry and effects.

  ![effect screenshot](img/four_simple.png)

  Note that you must run this example with @luajit@ since plain lua is not
  supported by four.

  ## Download source

  [SimpleShader.lua](examples/SimpleShader.lua)

--]]------------------------------------------------------
-- doc:literal

-- # Require
--
-- Every script must start by requiring lubyk to setup file paths, load some
-- globals such as "class" or "lk" and setup auto-loading of dependencies.
require 'lubyk'

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
  -- Index buffer (actual triangles defined with vertices in @vb@).
  local ib = four.Buffer { dim = 1,
             scalar_type = four.Buffer.UNSIGNED_INT }

  -- Create four vertices, one for each corner
  --
  --   #txt
  --   (-1, 1)              (1, 1)
  --     +--------------------+
  --     | 2                3 |
  --     |                    |
  --     |                    |
  --     | 0                1 |
  --     +--------------------+
  --   (-1,-1)              (1, -1)
  vb:push3D(-1.0, -1.0, 0.0)
  vb:push3D( 1.0, -1.0, 0.0)
  vb:push3D(-1.0,  1.0, 0.0)
  vb:push3D( 1.0,  1.0, 0.0)

  -- Colors for the positions above. The colors are not used in the
  -- final effect.
  cb:pushV4(four.Color.red())
  cb:pushV4(four.Color.green())
  cb:pushV4(four.Color.blue())
  cb:pushV4(four.Color.white())

  -- Create two triangles made of 3 vertices. Note that the index is
  -- zero based.
  ib:push3D(0, 1, 2)
  ib:push3D(1, 3, 2)

  -- Create the actual geometry object with four.Geometry.
  return four.Geometry {
    -- Declare the type of primitive to use (triangle here).
    primitive = four.Geometry.TRIANGLE, 
    -- Declare the index buffer.
    index = ib,
    -- And set data with vertex and color information.
    data = { vertex = vb, color = cb}
  }
end -- local function square()

-- Create the renderer with four.Renderer.
renderer = four.Renderer { size = four.V2(WIN_SIZE.w, WIN_SIZE.h) }

-- Use four.Camera to create a simple camera.
camera = four.Camera()

-- This is our renderable (an object that contains enough information to be
-- rendered by four.
obj = {
  -- This is a uniform declaration. The value of this variable will be
  -- accessible in the shader (see #Effect below).
  saturation = SATURATION,
  -- Set geometry by calling the method defined above.
  geometry   = square(),
}

-- # Effect
--
-- We create a new four.Effect that will process our geometry and make
-- something nice out of it. Apart from the uniforms declaration with
-- @default_uniforms@, the rest is simply GLSL code.
obj.effect = four.Effect {
  -- This declares the uniforms and sets default values in case the renderable
  -- (in our case @obj@) does not redefine these values.
  default_uniforms = {
    saturation = 0.5,
    -- This gives a 'time' uniform to our effect that reflects time in seconds
    -- (0 = script start).
    time = four.Effect.RENDER_FRAME_START_TIME,
  },

  -- Define the vertex shader. This shader simply passes values along to the
  -- fragment shader.
  vertex = four.Effect.Shader [[
    in vec4 vertex;
    in vec4 color;
    out vec4 v_vertex;
    // ignore color
    // out vec4 v_color;
    void main()
    {
      v_vertex = vertex;
      // v_color  = color;
      gl_Position = vertex;
    }
  ]],
  
  -- Define the fragment shader. This shader simply creates periodic colors
  -- based on pixel position and time.
  fragment = four.Effect.Shader [[
    in vec4 v_vertex;
    in vec4 v_color;

    // These uniform names must reflect the default_uniforms that we declared.
    uniform float saturation;
    uniform float time;
    float t = time;

    out vec4 color;

    void main() {

      vec4 v = 20 * (v_vertex + vec4(sin(t/10), sin(t/20), sin(t/30), 0.0));
      float r = saturation + 0.25 * (sin(v.x) + sin(v.y));
      float g = saturation + 0.25 * (sin(2*v.x) + sin(v.y));
      float b = saturation + 0.25 * (sin(3*v.x) + sin(v.y));
      color = vec4(b, r, g, 1);
    }
  ]]
}

-- # Window
--
-- We create an OpenGL window with mimas.GLWindow, set the size and position.
win = mimas.GLWindow()
win:resize(WIN_SIZE.w, WIN_SIZE.h)
win:move(WIN_POS.x, WIN_POS.y)
-- Show the window
win:show()

-- We then setup some simple keyboard actions to toggle fullscreen with
-- the space bar.
function win:keyboard(key, press)
  -- Only react to key press (press = true).
  if key == mimas.Key_Space and press then
    win:swapFullScreen()
  end
end

-- In case we resize the window, we want our content to scale so we need to
-- update the renderer's @size@ attribute.
function win:resized(w, h)
  renderer.size = four.V2(w, h)
end

-- OpenGL initialize function.
function win:initializeGL()
  -- What do this do ?
  renderer:logInfo()
end  

-- The window's paint function calls four.Renderer#render with our camera
-- and object.
function win:paintGL()
  renderer:render(camera, {obj})
end

-- ## Timer
-- Since our effect is a function of time, we update at 60 Hz. For this we
-- create a timer that asks the window to update every few milliseconds.
timer = lk.Timer(1/60, function()
  win:update()
  -- We also change the uniform's saturation with a random value between
  -- [0, 0.8] for a stupid blink effect.
  obj.saturation = math.random() * 0.8
end)

-- Start the timer.
timer:start()

-- And finally, run the scheduler (enter main loop).
run()



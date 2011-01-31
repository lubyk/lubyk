--[[------------------------------------------------------

  Use the wii.Remote to control OpenGL
  ------------------------------------

  Start the script and then press buttons '1' and '2' on
  the Remote to connect. The cube rotates depending on the
  data from the x, y and z axis. Press '1' or '2' to
  change the color, press '-' or '+' to change
  transparency. Press 'A' to start recording, 'A' again to
  stop and playback recorded data in a loop.

  Commands summary:

  1+2: discovery
  1: red
  2: blue
  +: opaque
  -: transparent
  A: record - playback - stop - record ...
  Home: quit

--]]------------------------------------------------------
require 'lubyk'

app = mimas.Application()

colors = {
  red    = {r=0.8, g=0.2, b=0.2},
  green  = {r=0.2, g=0.8, b=0.2},
  orange = {r=1.0, g=0.5, b=0.0},
  blue   = {r=0.2, g=0.2, b=0.8},
  pink   = {r=0.8, g=0.2, b=0.8},
  purple = {r=0.6, g=0.1, b=0.9}
}

function make_col(sy)
  local col = {}
  for i = 1,sy do
    table.insert(col, 0)
  end
  return col
end

function make_grid(sx, sy)
  local grid = {}
  for i = 1,sx do
    table.insert(grid, make_col(sy))
  end
  return grid
end

world = {
  size_x = {-3.0, 3.0, 80},  -- 200 = grid size
  size_y = {-2.0, 2.0, 60},
  color = colors.orange,
  -- 33 fps
  tick  = 30
}
-- compute real pos to grid ratio
function make_ratio(size)
  size[4] = size[3] / (size[2] - size[1])
end
make_ratio(world.size_x)
make_ratio(world.size_y)

function reset()
  world:reset()
  for _, snake in ipairs(snakes) do
    snake:reset()
  end
end

function world:reset()
  self.grid = make_grid(self.size_x[3] + 1, self.size_y[3] + 1)
end

world:reset()

Snake = {}
Snake.__index = Snake

setmetatable(Snake, {
  -- new method
 __call = function(table, id, color)
  local instance = {id = id, color = color, actions = {}}
  setmetatable(instance, Snake)
  instance:reset()
  return instance
end})

local function rand_pos(sz)
  return sz[1] + math.random() * (sz[2] - sz[1])
end

function Snake:reset()
  self.x = rand_pos(world.size_x)
  self.y = rand_pos(world.size_y)
  -- direction
  self.vx = 1
  self.vy = 0
  -- draw size
  self.sz = 0.05
  -- speed
  self.speed  = 0.0012 * world.tick
  -- color
  self.alpha = 0.8
  self.dead  = false
end

snakes = {Snake('Player 1', colors.orange), Snake('Player 2', colors.pink)}

function world:real_to_grid(x, y)
  local x = 1 + math.floor((x - self.size_x[1]) * self.size_x[4])
  local y = 1 + math.floor((y - self.size_y[1]) * self.size_y[4])
  return x, y
end

function world:bomb(snake)
  local x, y = self:real_to_grid(snake.x + 1.2 * snake.vx, snake.y + 1.2 * snake.vy)
  for i=-1,1 do
    local px = x + i
    if px < 0 then
      px = self.size_x[3] + px
    elseif px > self.size_x[3] then
      px = px - self.size_x[3]
    end
    for j=-1,1 do
      local py = y + j
      if py < 0 then
        py = self.size_y[3] + py
      elseif py > self.size_y[3] then
        py = py - self.size_y[3]
      end
      self.grid[px][py] = snake
    end
  end
end

function world:mark(snake)
  local x, y = self:real_to_grid(snake.x, snake.y)

  if snake.w_x ~= x or snake.w_y ~= y then
    -- moved
    -- store current grid position
    snake.w_x = x
    snake.w_y = y
    local old = self.grid[x][y]
    self.grid[x][y] = snake
    return old
  else
    -- on own grid position
    return 0
  end
end


function world:paint()
  gl.PushMatrix()
  local s_x = self.size_x
  local s_y = self.size_y

  gl.Color(self.color.r, self.color.g, self.color.b, 1.0)
  gl.LineWidth(1.0)
  gl.Begin('LINE_STRIP')
  	gl.Vertex(s_x[1], s_y[2]) -- Top Left
  	gl.Vertex(s_x[2], s_y[2]) -- Top Right
  	gl.Vertex(s_x[2], s_y[1]) -- Bottom Right
  	gl.Vertex(s_x[1], s_y[1]) -- Bottom Left
  	gl.Vertex(s_x[1], s_y[2]) -- Top Left
  gl.End()

  gl.Translate(s_x[1], s_y[1], 0)

  -- grid square size
  sx = 1 / self.size_x[4]
  sy = 1 / self.size_y[4]

  for x, col in ipairs(self.grid) do
    if x > self.size_x[3] then
      -- bug
      break
    end
    gl.Translate(sx, 0.0, 0.0)
    for y, pos in ipairs(col) do
      if y > self.size_y[3] then
        -- bug
        break
      end
      gl.Translate(0.0, sy, 0.0)
      if pos == 0 then
        -- draw in grey
        gl.Color(0.4, 0.4, 0.4, 0.4)
      else
        local color = pos.color
        gl.Color(color.r, color.g, color.b, pos.alpha)
      end

      -- we always draw empty cells to avoid slowdown when things get full
      gl.Begin('QUADS') -- triangle strip would be better...
      	gl.Vertex(  0,   0) -- Top Right
      	gl.Vertex(  0, -sy) -- Bottom Right
      	gl.Vertex(-sx, -sy) -- Bottom Left
        gl.Vertex(-sx,   0) -- Top Left
      gl.End()
    end
    gl.Translate(0.0, -self.size_y[3] * sy, 0.0)
  end
  gl.PopMatrix()
end

local function world_map(pos, size)
  if pos < size[1] then
    return size[2]
  elseif pos >= size[2] then
    return size[1]
  else
    return pos
  end
end

function Snake:step()
  if self.dead then
    return
  end
  self.x = world_map(self.x + self.vx * self.speed, world.size_x)
  self.y = world_map(self.y + self.vy * self.speed, world.size_y)
  if world:mark(self) ~= 0 then
    -- dead
    self.dead  = true
    self.alpha = 0.3
  end
end

function Snake:paint()
  gl.PushMatrix()
  -- draw our head
  local x, y = world:real_to_grid(self.x, self.y)
  local sx = 1 / world.size_x[4]
  local sy = 1 / world.size_y[4]

  -- the world is off ?
  gl.Translate(world.size_x[1] + x * sx, world.size_y[1] + y * sy, 0)

  local color = self.color
  gl.Color(color.r, color.g, color.b, 1.0)
  gl.Begin('QUADS') -- triangle strip would be better...
  	gl.Vertex(  0,   0) -- Top Right
  	gl.Vertex(  0, -sy) -- Bottom Right
  	gl.Vertex(-sx, -sy) -- Bottom Left
    gl.Vertex(-sx,   0) -- Top Left
  gl.End()
  gl.PopMatrix()
end

function Snake:learn()
  print('--------------------------')
  print('Learning actions for "' .. self.id .. '"')
  local actions = {'UP', 'RIGHT', 'DOWN', 'LEFT', 'FIRE'}
  for _, action in ipairs(actions) do
    self.learn_action = action
    io.write(string.format('%8s..', action))
    io.flush()
    while self.learn_action do
      sleep(30)
    end
    print('OK')
  end
end

function Snake:action(event)
  if self.learn_action then
    self.actions[event] = self.learn_action
    self.learn_action = nil
  else
    local action = self.actions[event]
    if not action then
      return false
    elseif action == 'UP' then
      self.vy = 1
      self.vx = 0
    elseif action == 'RIGHT' then
      self.vy = 0
      self.vx = 1
    elseif action == 'DOWN' then
      self.vy = -1
      self.vx = 0
    elseif action == 'LEFT' then
      self.vy = 0
      self.vx = -1
    elseif action == 'FIRE' then
      world:bomb(self)
    end
  end
end

win = mimas.GLWindow()

-- cube animation (button B)
anim = rk.Timer(world.tick, function()
  for _, snake in ipairs(snakes) do
    snake:step()
  end
  win:updateGL()
end)

function win.initializeGL()
  gl.Enable("POINT_SMOOTH")
  gl.Enable("SMOOTH")
  gl.Enable("BLEND")                                -- Enable alpha blending
  gl.BlendFunc("SRC_ALPHA", "ONE_MINUS_SRC_ALPHA")  -- Set blend function

  gl.ClearDepth(1.0)
  gl.DepthFunc("LEQUAL");

  -- glEnable(GL_CULL_FACE);
  -- glEnable(GL_DEPTH_TEST);

  gl.Hint("PERSPECTIVE_CORRECTION_HINT", "NICEST") -- Really nice perspective
  gl.ClearColor(0.2,0.2,0.2,0.01)
end

function win.resizeGL(width, height)
  gl.Enable("BLEND")
  gl.Disable("DEPTH_TEST")
  gl.BlendFunc("SRC_ALPHA", "ONE_MINUS_SRC_ALPHA")

  gl.Enable("LINE_SMOOTH")
  -- Select the projection matrix
  gl.MatrixMode("PROJECTION")
  -- reset
  gl.LoadIdentity()
  -- Calculate the aspect ratio of the view
  gl.Perspective(
    45,               -- Field of view angle
    width / height,   -- Aspect ration
    1,                -- zNear
    100               -- zFar
  )
end

function win.paintGL()
  gl.Clear( "COLOR_BUFFER_BIT, DEPTH_BUFFER_BIT")
  gl.MatrixMode("MODELVIEW")
  gl.LoadIdentity()

  gl.Translate(0, 0, -6.0)

  world:paint()

  for _, snake in ipairs(snakes) do
    snake:paint()
  end
end

function win.keyboard(key, on, txt)
  if on then
    if key == 16777216 then
      -- ESC
      reset()
    else
      for _, snake in ipairs(snakes) do
        if snake:action(key) then
          break
        end
      end
    end
  end
end
win:show()

browser = wii.Browser(function(found_wii)
  wiimote = found_wii
  function wiimote.acceleration(device, lx, ly, lz)
    -- noop
  end

  function wiimote.button(btn, on)
    if on then
      if btn == 'Remote.H' then
        app:quit()
      else
        for _, snake in ipairs(snakes) do
          if snake:action(btn) then
            break
          end
        end
      end
    end
  end
end)

app:post(function()
  learning = rk.Thread(function()
    for _, snake in ipairs(snakes) do
      snake:learn()
    end
    print('-----------')
    print('done.')
    anim:start()
  end)
end)

app:exec()

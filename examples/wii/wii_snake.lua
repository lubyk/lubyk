--[[------------------------------------------------------

  Use the wii.Remote or keyboard to control the snakes
  ----------------------------------------------------

  Start the script and then press buttons '1' and '2' on
  the Remote to connect. Follow the instructions to set
  the controls for Player1 and Player2.

  Special commands:
  +: new game
  Home: quit

--]]------------------------------------------------------
require 'lubyk'

app = mimas.Application()

--============================================== constants

colors = {
  red    = {r=0.8, g=0.2, b=0.2},
  green  = {r=0.2, g=0.8, b=0.2},
  orange = {r=1.0, g=0.5, b=0.0},
  blue   = {r=0.2, g=0.2, b=0.8},
  pink   = {r=0.8, g=0.2, b=0.8},
  purple = {r=0.6, g=0.1, b=0.9}
}

--============================================== wii.Remote

wiimotes = {}
for wiimote_id=1,2 do
  local wiimote = wii.Remote()
  wiimotes[wiimote_id] = wiimote
  function wiimote.button(btn, on)
    if on then
      if btn == 'Remote.H' then
        app:quit()
      elseif btn == 'Remote.+' then
        reset()
      else
        -- differenciate between remote 1 and remote 2
        btn = string.format('%s.%i', btn, wiimote_id)
        for _, snake in ipairs(snakes) do
          if snake:action(btn) then
            break
          end
        end
      end
    end
  end
end

--============================================== Sound

effect = {mi = midi.Out('lubyk')}
print(effect.mi)
effect.sounds = {
  die  = {chan = 1, note = 24, velocity = 60},
  turn = {chan = 1, note = 25, velocity = 30},
  fire = {chan = 1, note = 26, velocity = 30},
}

function effect:play(key)
  local sound = self.sounds[key]
  if not sound then
    print('Unknown effect', key)
    return
  end
  self.mi:send(143 + sound.chan, sound.note, sound.velocity)
end
--============================================== World
World = {}
World.__index = World

setmetatable(World, {
  -- new method
 __call = function(table, base)
  local instance = base
  setmetatable(instance, World)
  instance:reset()
  return instance
end})

-- Helper to build a single column in the world grid.
local function make_col(sy)
  local col = {}
  for i = 1,sy do
    table.insert(col, 0)
  end
  return col
end

-- Build a grid for the pixels that can be occupied or left empty.
local function make_grid(sx, sy)
  local grid = {}
  for i = 1,sx do
    table.insert(grid, make_col(sy))
  end
  return grid
end

-- compute real pos to grid ratio
local function make_ratio(size)
  size[4] = size[3] / (size[2] - size[1])
end

function World:reset()
  self.grid = make_grid(self.size_x[3] + 1, self.size_y[3] + 1)
  make_ratio(self.size_x)
  make_ratio(self.size_y)
end

function World:real_to_grid(x, y)
  local x = 1 + math.floor((x - self.size_x[1]) * self.size_x[4])
  local y = 1 + math.floor((y - self.size_y[1]) * self.size_y[4])
  return x, y
end

function World:bomb(snake)
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

function World:mark(snake)
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


function World:paint()
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

--============================================== Snake

Snake = {}
Snake.__index = Snake

setmetatable(Snake, {
  -- new method
 __call = function(table, id, world, color)
  local instance = {id = id, world = world, color = color, actions = {}}
  setmetatable(instance, Snake)
  instance:reset()
  return instance
end})

local function rand_pos(sz)
  return sz[1] + math.random() * (sz[2] - sz[1])
end

function Snake:reset()
  self.x = rand_pos(self.world.size_x)
  self.y = rand_pos(self.world.size_y)
  -- direction
  self.vx = 1
  self.vy = 0
  -- draw size
  self.sz = 0.05
  -- speed
  self.speed  = 0.0012 * self.world.tick
  -- color
  self.alpha = 0.8
  self.dead  = false
end

function Snake:step()
  if self.dead then
    return
  end
  self.x = world_map(self.x + self.vx * self.speed, self.world.size_x)
  self.y = world_map(self.y + self.vy * self.speed, self.world.size_y)
  if self.world:mark(self) ~= 0 then
    -- dead
    effect:play('die')
    self.dead  = true
    self.alpha = 0.3
  end
end

function Snake:paint()
  gl.PushMatrix()
  -- draw our head
  local x, y = self.world:real_to_grid(self.x, self.y)
  local sx = 1 / self.world.size_x[4]
  local sy = 1 / self.world.size_y[4]

  -- the world is off ?
  gl.Translate(self.world.size_x[1] + x * sx, self.world.size_y[1] + y * sy, 0)

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
      effect:play('turn')
    elseif action == 'RIGHT' then
      self.vy = 0
      self.vx = 1
      effect:play('turn')
    elseif action == 'DOWN' then
      self.vy = -1
      self.vx = 0
      effect:play('turn')
    elseif action == 'LEFT' then
      self.vy = 0
      self.vx = -1
      effect:play('turn')
    elseif action == 'FIRE' then
      self.world:bomb(self)
      effect:play('fire')
    end
  end
end

--============================================== OpenGL

win = mimas.GLWindow()

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
    if key == mimas.ESC then
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

--============================================== wii.Remote

wiimotes = {}
for wiimote_id=1,2 do
  local wiimote = wii.Remote()
  wiimotes[wiimote_id] = wiimote
  function wiimote.button(btn, on)
    if on then
      if btn == 'Remote.H' then
        app:quit()
      elseif btn == 'Remote.+' then
        reset()
      else
        -- differenciate between remote 1 and remote 2
        btn = string.format('%s.%i', btn, wiimote_id)
        for _, snake in ipairs(snakes) do
          if snake:action(btn) then
            break
          end
        end
      end
    end
  end
end

--============================================== Main

world = World{
  size_x = {-3.0, 3.0, 80},  -- 200 = grid size
  size_y = {-2.0, 2.0, 60},
  color = colors.orange,
  -- 33 fps
  tick  = 30
}

snakes = {Snake('Player 1', world, colors.orange), Snake('Player 2', world, colors.pink)}

function reset()
  world:reset()
  for _, snake in ipairs(snakes) do
    snake:reset()
  end
end

world:reset()
win:show()

-- game main loop
anim = lk.Timer(world.tick, function()
  for _, snake in ipairs(snakes) do
    snake:step()
  end
  win:updateGL()
end)

app:post(function()
  learning = lk.Thread(function()
    for _, snake in ipairs(snakes) do
      snake:learn()
    end
    print('-----------')
    print('done.')
    anim:start()
  end)
end)

app:exec()

--[[------------------------------------------------------

  counter
  -------

  Send increasing numbers on each bang.

--]]------------------------------------------------------

inlet('tic', 'Advance animation.')

mouse = {x = 0, y = 0}

function make_dots(count)
  local grid = {
    [1] = {x = 200, y = 200, vx = 0, vy = 0, l = {2, 3, 4}},
    [2] = {x = 110, y = 300, vx = 0, vy = 0, l = {1, 3, 4}},
    [3] = {x = 300, y = 320, vx = 0, vy = 0, l = {1, 2, 4}},
    [4] = {x = 340, y = 220, vx = 5, vy = 0, l = {1, 2, 3}},
  }
  for _, elem in ipairs(grid) do
    print(_)
    local links = elem.l
    local i = 0
    for i, link_id in ipairs(links) do
      links[i] = grid[link_id]
    end
  end
  return grid
end

dots = {
  old = make_dots(3),
  new = make_dots(3)
}

local RADIUS = 90
local K = 0.1
local MASS = 0.1
local FRICTION = 10
local REPULSION = 0.05

function compute_elem(self, old, dt)
  local force = {x = 0, y = 0}
  for _, other in ipairs(old.l) do
    -- distance to other
    local dx = other.x - old.x
    local dy = other.y - old.y
    -- attraction
    force.x = force.x + K * dx
    force.y = force.y + K * dy
    -- repulsion
    local norm = math.sqrt(dx*dx + dy*dy) - 2 * RADIUS
    if norm < 0 then
      force.x = force.x + REPULSION * norm * dx
      force.y = force.y + REPULSION * norm * dy
    end
  end
  -- friction
  force.x = force.x - old.vx * dt * FRICTION
  force.y = force.y - old.vy * dt * FRICTION
  self.vx = old.vx + dt * force.x / MASS
  self.vy = old.vy + dt * force.y / MASS
  self.x  = old.x + self.vx * dt
  self.y  = old.y + self.vy * dt
end

function dots:move(dt)
  local old = self.old
  local new = self.new
  for i, old_elem in ipairs(old) do
    compute_elem(new[i], old_elem, dt)
  end
  -- swap
  self.old, self.new = new, old
end

function inlet.tic()
  dots:move(0.01)
  win:update()
end

if not win then
  win = mimas.Widget()
  width  = 300
  height = 300
  win:resize(width,height)
  win:move(10,10)
  win:show()
end

math.randomseed(os.time())

-- do something with mouse input
function win.mouse(x, y)
  dots.old[1].x = x
  dots.old[1].y = y
  win:update()
end

function win.keyboard(key, on)
  if on then
    if key == mimas.ESC then
      -- ESC
      win:close()
    elseif key == mimas.Space then
      win:swapFullScreen()
    end
  end
end


function win.paint(p, w, h)
  local path = mimas.Path()
  local ox, oy = 0, 0
  for i, elem in ipairs(dots.new) do
    if i == 1 then
      path:moveTo(elem.x, elem.y)
    else
      local dx, dy = ox - elem.x, oy - elem.y
      local norm = math.sqrt(dx*dx + dy*dy)
      local dx, dy = dx/norm, dy/norm
      path:cubicTo(ox + RADIUS * dx, oy + RADIUS * dy/norm, elem.x - RADIUS * dx/norm, elem.y - RADIUS, elem.x, elem.y)
    end
    ox, oy = elem.x, elem.y
    p:setBrush(mimas.Color(0.2, 1, 1, 0.5))
    p:setPen(1, mimas.Color(0.2, 0.8, 0.8, 0.8))
   -- p:drawEllipse(elem.x - RADIUS, elem.y - RADIUS, 2*RADIUS, 2*RADIUS)
  end

  p:drawPath(path)
end
win:update()
--app:exec()

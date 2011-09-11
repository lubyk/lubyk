require 'lubyk'

app = app or mimas.Application()

if not win then
  win = mimas.Widget()
  win:setMouseTracking(true)
  width  = 300
  height = 300
  win:resize(width,height)
  win:move(10,10)
  win:show()
end

math.randomseed(os.time())
-- distance from planet to moon

dist   = math.min(width, height) * 0.75
planet = planet or {cx=width/2,cy=height/2}
moons = {}
moon_count = 2

local function make_moons()
  moon_count = moon_count + 1
  for i=1,moon_count do
    local hue = math.random()
    moons[i] = {
      cx    = 200,
      cy    = 150,
      r     = 3+dist*math.random()/8,
      a     = 6.28*math.random(),
      v     = 0.02*math.random(),
      dist  = 15+dist*math.random(),
      pen   = mimas.Pen(2, hue, 0.8, 0.8, 0.8),
      brush = mimas.Brush(hue, 0.5, 0.5, 0.5),
    }
  end
end
if not moons[1] then
  make_moons()
end

-- super action on mouse move
local function move(x, y)
  planet.cx = x
  planet.cy = y
  win:update()
end

-- do something with mouse input
function win:mouse(x, y)
  move(x, y)
end

function win:keyboard(key, on)
  if on then
    if key == mimas.ESC then
      -- ESC
      win:close()
    elseif key == mimas.Space then
      win:swapFullScreen()
    else
      make_moons()
    end
  end
end

function win:click()
  make_moons()
end

function win:resized(w, h)
  width  = w
  height = h
  dist = math.min(w, h) * 0.75
end

function win:paint(p, w, h)
  for i=1,moon_count do
    local moon = moons[i]
    p:setBrush(moon.brush)
    p:setPen(moon.pen)
    local r = moon.r
    p:drawEllipse(moon.cx - r, moon.cy - r, 2*r, 2*r)
  end
end

timer = timer or lk.Timer(10, function()
  -- advance angle by 0.04 radians
  for i=1,moon_count do
    local moon = moons[i]
    moon.a = moon.a + moon.v
    moon.cx = planet.cx + moon.dist * math.cos(moon.a)
    moon.cy = planet.cy + moon.dist * math.sin(moon.a)
  end
  app:post(function()
    win:update()
  end)
end)
timer:start()

app:exec()
timer:stop()

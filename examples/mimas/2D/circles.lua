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

r   = 30
repeat_color = 0.5 -- half window diagonal
pad = 8
mouse = mouse or {x=0,y=0}

-- super action on mouse move
local function move(x, y)
  mouse.x = x
  mouse.y = y
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
    end
  end
end

function win:paint(p, w, h)
  local x_count = math.floor(w/(2*r+pad)) + 1
  local y_count = math.floor(h/(2*r+pad)) + 1
  local diag = math.sqrt(w*w + h*h) * 0.7
  local cx = r + pad/2
  for i=1,x_count do
    local cy = r + pad/2
    for j=1,y_count do
      local dx = mouse.x - cx
      local dy = mouse.y - cy
      local dist = math.sqrt(dx*dx + dy*dy)
      local hue = (dist / diag) % 1.0
      if dist < r then
        p:setBrush(mimas.Color(hue, 0.8, 0.8, 0.8))
        p:setPen(4, mimas.Color(hue, 1, 1, 1))
      else
        p:setBrush(mimas.Color(hue, 0.5, 0.5, 0.5))
        p:setPen(2, mimas.Color(hue, 0.8, 0.8, 0.8))
      end
      p:drawEllipse(cx - r, cy - r, 2*r, 2*r)
      cy = cy + 2*r + pad
    end
    cx = cx + 2*r + pad
  end
end
win:update()
run()

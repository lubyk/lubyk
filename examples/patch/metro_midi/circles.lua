--[[------------------------------------------------------

  circles
  -------

  Draw stupid fading circles.

--]]------------------------------------------------------
inlet('hue', 'Receive hue for new circle [float].')

tim = tim or lk.Timer(40)
tim:start()

win = win or mimas.Window()
win:show()

hist = hist or {}

function inlet.hue(hue)
  table.insert(hist, {hue = hue, cx = hue*1.5, cy = 0.2, r = 0.2})
end

function win.paint(p, w, h)
  --p:fillRect(0, 0, w, h, mimas.Color(hue))
  local dx = w/2
  local dy = h/2
  local dr = math.min(w/3,h/3)

  for _, circle in ipairs(hist) do
    local r = circle.r * dr * 0.5
    p:setBrush(mimas.Color(circle.hue, 0.8, 0.8, 0.3))
    p:setPen(2, mimas.Color(circle.hue, 1, 1, 1))
    p:drawEllipse(dx*circle.cx - r, dy*circle.cy - r, 2*r, 2*r)
  end
  while #hist > 50 do
    table.remove(hist, 1)
  end
end

function tim.tick()
  for _, circle in ipairs(hist) do
    circle.cx = circle.cx * 1.007
    circle.cy = circle.cy * 1.02
    circle.r  = circle.r * 1.02
  end
  win:update()
end
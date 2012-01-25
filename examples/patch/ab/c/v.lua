inlet('animate')

-- Default values for parameters. This also declares which values have to be
-- exported in a dump operation (to get a settings snapshot for example).
defaults {
  x    = 0.5,
  y    = 0.5,
  size = 0.5,
  hue  = 0.3,
  shape= 'rect',
}

function paramChanged()
  win:update()
end
  

local TIME_SCALE = math.pi / 1000 -- one rotation per second
local sin, abs, min = math.sin, math.abs, math.min

if not win then
  win = mimas.Window()
  win:show()
  win:move(10,10)
  win:resize(300,300)
end

t = t or 0

function win:paint(p, w, h)
  local px = w * x
  local r = min(w, h) * size / 2
  local height = h - 2 * r - 7
  local py = h - (height * y) - r
  p:setBrush(mimas.Color(hue, 0.5, 0.5))
  p:setPen(7, mimas.Color(hue, 0.8))
  if shape == 'rect' then
    p:drawRect(px - r, py - r, 2 * r, 2 * r)
  else
    p:drawEllipse(px - r, py - r, 2 * r, 2 * r)
  end
end
win:update()

function inlet.animate(v)
  t = v
  param.y = abs(sin(t * TIME_SCALE))
  win:update()
end

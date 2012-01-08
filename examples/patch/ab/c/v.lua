inlet('win')

-- Parameters that are always notified with script changes and that
-- are recorded on settings save.
defaults {
  x    = 0.5,
  y    = 0.5,
  size = 0.5,
}

if not win then
  win = mimas.Window()
  win:show()
end

function win:paint(p, w, h)
  local px, py = w * x, h * y
  local r = math.min(w, h) * size / 2
  p:setBrush(mimas.Color(hue, 0.3, 0.3))
  p:setPen(2, mimas.Color(hue, 0.5, 0.5))
  p:drawEllipse(px - r, py - r, 2 * r, 2 * r)
end
win:update()

function inlet.win(v)
  hue = v / 80 % 1.0
  win:update()
end

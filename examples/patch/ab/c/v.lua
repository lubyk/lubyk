inlet('win')

if not win then
  win = mimas.Window()
  win:show()
end

function win:paint(p, w, h)
  p:fillRect(0, 0, w, h, mimas.Color(hue))
end
win:update()

function inlet.win(v)
  hue = v / 100 % 1.0
  win:update()
end

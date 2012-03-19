require 'lubyk'
require 'mimas'

win = mimas.Window()
win:show()
win.hue = 0.3
win.sat = 0.5

function win:paint(p, w, h)
  p:fillRect(0, 0, w, h, mimas.Color(win.hue, win.sat))
end

function win:click(x, y)
  win.hue = x / win:width()
  win.sat = y / win:height()
  win:update()
end

win.mouse = win.click

run()

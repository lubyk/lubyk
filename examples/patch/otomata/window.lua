--[[------------------------------------------------------

  window
  ------

  Window to draw the automaton.

--]]------------------------------------------------------
inlet('update', 'Update draw [bang].')
paint = outlet('paint', 'Painter to draw [painter, width, height].')

win = win or mimas.Window()
win:show()

function inlet.update()
  win:update()
end

function win.paint(p, w, h)
  paint(p, w, h)
end

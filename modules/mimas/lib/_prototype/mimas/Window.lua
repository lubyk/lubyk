--[[------------------------------------------------------

  mimas.Window
  ------------

  Display a window and uses it's outlet to ask for paint
  operations in other (local) nodes.

--]]------------------------------------------------------
inlet('update', 'Redraw window content')
inlet('x', 'x position')
inlet('y', 'y position')
inlet('w', 'width')
inlet('h', 'height')

outlet('paint', 'Asks subnodes to paint in the provided region (painter, x, y, w, h)')

defaults {
  x = 0,
  y = 0,
  w = 600,
  h = 400,
}

if not win then
  win = mimas.Window()
  win:move(x, y)
  win:resize(w, h)
  win:show()
end

--=============================================== INLETS
function inlet.x(v)
  x = v
  win:move(x, y)
end

function inlet.y(v)
  y = v
  win:move(x, y)
end

function inlet.w(v)
  w = v
  win:resize(w, h)
end

function inlet.h(v)
  h = v
  win:resize(w, h)
end

function inlet.update()
  win:update()
end

--=============================================== Window callbacks

function win:moved(vx, vy)
  param.x = vx
  param.y = vy
end

function win:resized(vw, vh)
  param.w = vw
  param.h = vh
end

function win:paint(p, w, h)
  paint(p, 0, 0, w, h)
end

--[[------------------------------------------------------

  play
  ----

  When the object receives data, it just prints it out.

--]]------------------------------------------------------

inlet('trigger', 'Print data as yaml [anything].')
if not win then
  win = mimas.Window()
  win:show()
end

t = t or 0
function win:paint(p, w, h)
  local x = -45 + w/2 + math.cos(10 * t) * w / 3
  local y = -45 + h/2 + math.sin(10 * t) * h / 3
  p:setBrush(mimas.Color(0, 0.8, 0.8, 0.3))
  p:setPen(3, mimas.Color(0.3, 0.8, 0.8, 0.5))
  p:drawRoundedRect(x, y, 90, 90, 10, 10)
  win:update()
end
win:update()
  

function inlet.trigger(nb, msg)
  t = nb/1000
  print(t)
end

print('Reloaded "play.lua"')

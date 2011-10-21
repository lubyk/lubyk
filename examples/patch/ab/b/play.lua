--[[------------------------------------------------------

  play
  ----

  When the object receives data, it just prints it out.

--]]------------------------------------------------------

inlet('trigger', 'Print data as yaml [anything].')

t = t or 0
function inlet.trigger(nb)
  print(nb)
end

print('Reloaded "play.lua"')

--[[------------------------------------------------------

  play
  ----

  When the object receives data, it just prints it out.

--]]------------------------------------------------------

inlet('trigger', 'Print data as yaml [anything].')

function inlet.trigger(...)
  print(...)
end

print('Reloaded "play.lua"')
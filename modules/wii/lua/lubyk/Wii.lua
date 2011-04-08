--[[------------------------------------------------------

  lubyk.Wii
  ---------

  Sends acceleration and btn information out.

--]]------------------------------------------------------
acceleration = outlet('acceleration', 'Sends x,y,z acceleration [float,float,float].')
button = outlet('button', 'Sends button press information [string,bool].')

wiimote = wiimote or wii.Remote()

function wiimote.button(btn, pressed)
  -- button callback
  button(btn, pressed)
end

function wiimote.acceleration(device, x, y, z)
  acceleration(x, y, z)
end

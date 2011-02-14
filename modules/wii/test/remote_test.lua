--[[------------------------------------------------------

  wii.Remote
  ----------

  Access wii Remote.

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('wii.Remote')


function should.set_button_callback()
  local remote = wii.Remote()

  assert_pass(function()
    function remote.button(btn, pressed)
      -- button callback
    end
  end)
end

function should.set_acceleration_callback()
  local remote = wii.Remote('Foo')

  assert_pass(function()
    function remote.acceleration(device, x, y, z)
      -- button callback
    end
  end)
end

test.all()
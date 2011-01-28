--[[------------------------------------------------------

  wii.Remote
  ----------

  Access wii Remote.

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('wii.Remote')

function should.load_wii_code()
  assert_true(true)
end

function should.find_first_remote()
  local remote = wii.Remote('Foo')

  assert_match('Foo', remote:name())
  --assert_true(remote:connected())
end

function should.set_button_callback()
  local remote = wii.Remote('Foo')

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
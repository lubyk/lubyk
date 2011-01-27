--[[------------------------------------------------------

  wii.Browser
  -----------

  Browse for wii remotes.

--]]------------------------------------------------------
require 'lubyk'
require 'mimas'
local should = test.Suite('wii.Browser')

function should.load_wii_code()
  assert_true(wii)
end

function should.find_first_remote(t)
  -- this will not work until we have an event loop running on OS X.
  t.remote = wii.Browser(function(found_wii)
    print('Found', found_wii)
    t.wiimote = found_wii
    print('Set leds')
    found_wii:set_leds(false, true, false, true)
    function t.wiimote.button(name,state)
      print(name, state)
    end
  end)

  worker:run()
  while not t.wiimote do
    print('.')
    sleep(100)
  end
  assert_true(t.wiimote)
end

test.all()
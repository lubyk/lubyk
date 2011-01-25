--[[------------------------------------------------------

  wii.Browser
  -----------

  Browse for wii remotes.

--]]------------------------------------------------------
require 'rubyk'
require 'mimas'
local should = test.Suite('wii.Browser')

function should.load_wii_code()
  assert_true(wii)
end

function should.find_first_remote(t)
  -- this will not work until we have an event loop running on OS X.
  t.remote = wii.Browser(function(found_wii)
    print('found', found_wii)
    t.wiimote = found_wii
  end)

  worker:run()
  while not t.wiimote do
    print('.')
    sleep(100)
  end
  assert_true(t.wiimote)
end

test.all()
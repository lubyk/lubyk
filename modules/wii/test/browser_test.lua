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
  t.win = mimas.Window()
  t.lay = mimas.VBoxLayout(t.win)
  t.title = mimas.Label('wii.Remote: Press 1+2')
  t.lay:addWidget(t.title)
  t.label = mimas.Label('')
  t.lay:addWidget(t.label)
  t.win:show()
  t.continue = false

  -- this will not work until we have an event loop running on OS X.
  t.remote = wii.Browser(function(found_wii)
    t.wiimote = found_wii
    t.title:setText('Connected. Press home to stop.')
    function t.wiimote.button(name,state)
      t.wiimote:set_leds(false, true, false, true)
      if state then
        t.label:setText(name .. ': ON')
      else
        t.label:setText(name .. ': OFF')
      end

      if name == 'Remote.H' then
        t.continue = true
      end
    end
  end)
  t.now = worker:now()
  t.test = lk.Thread(function()
    while not t.continue and (worker:now() < t.now + 2000 or t.wiimote) do
      sleep(20)
    end
    if not t.wiimote then
      print("No wiimote test")
    end
    assert_true(true)
    t.win:close()
  end)
end

test.gui()
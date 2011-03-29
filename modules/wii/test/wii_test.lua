--[[------------------------------------------------------

  wii.Browser
  -----------

  Browse for wii remotes.

--]]------------------------------------------------------
require 'lubyk'
require 'mimas'
local should = test.Suite('wii.Browser')

function should.load_wii_code()
  assertTrue(wii)
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
  t.remote  = wii.Remote()
  t.remote2 = wii.Remote()

  function t.remote.button(name, state)
    t.remote:set_leds(false, true, false, true)
    if state then
      t.label:setText(name .. ': ON')
    else
      t.label:setText(name .. ': OFF')
    end

    if name == 'Remote.H' then
      t.continue = true
    end
  end

  function t.remote.connected()
    t.remote:set_leds(true, false, false, false)
    t.title:setText('Connected. Press home to stop.')
  end

  function t.remote2.connected()
    t.remote2:set_leds(false, true, false, false)
    t.title:setText('Connected [2]')
  end

  t.now = worker:now()
  t.test = lk.Thread(function()
    while not t.continue and (worker:now() < t.now + 20000 or t.wiimote) do
      sleep(20)
    end
    if not t.continue then
      print("No wiimote found")
    end
    assertTrue(true)
    t.win:close()
  end)
end

test.gui()
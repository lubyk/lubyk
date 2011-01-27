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
  local remote = wii.Remote("Foobar")

  assert_match('Foobar', remote:name())
  --assert_true(remote:connected())
end

function should.set_callbacks()
  local remote = wii.Remote("Foobar")

  function remote:button(btn, pressed)
    print(btn, pressed)
  end
  --assert_true(remote:connected())
end

test.all()
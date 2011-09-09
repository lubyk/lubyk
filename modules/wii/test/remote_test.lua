--[[------------------------------------------------------

  wii.Remote
  ----------

  Access wii Remote.

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('wii.Remote')
local withUser = should:testWithUser()

function should.setButtonCallback()
  local remote = wii.Remote()

  assertPass(function()
    function remote:button(btn, pressed)
      -- button callback
    end
  end)
end

function should.setAccelerationCallback()
  local remote = wii.Remote('Foo')

  assertPass(function()
    function remote:acceleration(device, x, y, z)
      -- button callback
    end
  end)
end

function withUser.should.receiveAcceleration(t)
  local remote = wii.Remote('Foo')
  function remote:acceleration(device, x, y, z)
    print(device, x, y, z)
  end

  function remote:button()
    t.continue = true
  end

  t:timeout(function(done)
    if done or t.continue then
      assertTrue(t.continue)
      return true
    end
  end)
end

test.all()

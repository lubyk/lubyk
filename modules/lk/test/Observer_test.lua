--[[------------------------------------------------------

  lk.Observer test
  ----------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.Observer')

--=============================================== [Dummy]
-- A dummy class to use for testing
local Dummy = {}
Dummy.__index = Dummy

setmetatable(Dummy, {
  __call = function(lib, name)
    self = {name = name}
    return setmetatable(self, Dummy)
  end
})

function Dummy:observed(other, action)
  local observations = self.observations
  if not observations then
    observations = {}
    self.observations = observations
  end
  table.insert(observations, {other,action})
end

function Dummy:setName(name)
  self.name = name
  self:notify('changed')
end
-- Enable observer pattern to the Dummy class.
lk.Observer.mixin(Dummy)
--=============================================== [/Dummy]

function should.respondToObserve()
  local alice = Dummy('Alice')
  local bob   = Dummy('Bob')
  assertPass(function()
    alice:observe(bob)
  end)
end

function should.callObserverOnNotify()
  local alice = Dummy('Alice')
  local bob   = Dummy('Bob')
  alice:observe(bob)
  bob:notify('hello')
  assertEqual(bob, alice.observations[1][1])
  assertEqual('hello', alice.observations[1][2])
end

function should.callObserverOnlyOnce()
  local alice = Dummy('Alice')
  local bob   = Dummy('Bob')
  alice:observe(bob)
  alice:observe(bob)
  bob:notify('hello')
  assertEqual(1, #alice.observations)
  assertEqual(bob, alice.observations[1][1])
  assertEqual('hello', alice.observations[1][2])
end

function should.notRetainObservers()
  local alice = Dummy('Alice')
  local bob   = Dummy('Bob')
  local deleted
  alice.fin = lk.Finalizer(function()
    deleted = true
  end)
  alice:observe(bob)
  alice = nil
  collectgarbage('collect')
  collectgarbage('collect')
  assertTrue(deleted)
end

test.all()

--[[------------------------------------------------------

  lk.Observer
  -----------

  This enables observer patterns between classes. This is
  like a mixin and should be used when defining a class:

    lk.Observer.mixin(lk.Node)

  This transforms all instances of 'lk.Node' into an
  observers and also makes it them observable. Note that
  the given class ('lk.Node' in this example) should
  implement 'observed(self, instance, event)'.

--]]------------------------------------------------------
local lib     = {type='lk.Observer'}
lib.__index   = lib
lk.Observer   = lib
local private = {}


function lib.mixin(class)
  assert(class.observed, 'The observed class should implement "observed(self, instance, event)"')
  assert(not class.notify, 'Existing "notify" method ("notify" is used by lk.Observer).')
  class.notify  = private.notify
  class.observe = private.observe
end

--- Start observing another class. Whenever the class notifies
-- our '
function private:observe(instance)
  local observers = instance.observers
  if not observers then
    instance.observers = setmetatable({}, {__mode = 'k'})
    observers = instance.observers
  end

  -- We store observers in keys so that we are sure to only register
  -- each observer only once.
  observers[self] = true
end

--- Whenever an event happens that should be notified to observers, this
-- method should be called.
function private:notify(action)
  local observers = self.observers
  if observers then
    for observer, _ in pairs(observers) do
      observer:observed(self, action)
    end
  end
end


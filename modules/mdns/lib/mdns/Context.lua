--[[------------------------------------------------------

  mdns.Context
  ------------

  This class is used internally.

--]]------------------------------------------------------
require 'mdns.core'
local lib     = mdns.Context_core
mdns.Context  = lib
local WeakKey = {__mode = 'k'}

local new = lib.new
function lib.new()
  local self = new()
  if not self.callbacks then
    self.callbacks = {}
  end
  self.registrations = setmetatable({}, WeakKey)
  -- Installs the file descriptor callbacks (linux only) and start
  -- browsing. This will call 'start' from the C++ side.
  self:run()
  return self
end

function lib:addSelectCallback(clbk)
  if not self.callbacks then
    self.callbacks = {}
  end
  -- Insertion into the sceduler will be done in lk.SelectCallback.update.

  -- Protect from GC
  self.callbacks[clbk] = clbk
  -- Let it die (this is called by lk.SelectCallback.remove).
  function clbk.finalize()
    self.callbacks[clbk] = nil
  end
end

--=============================================== Called by mdns.Registration

function lib:addRegistration(reg)
  if self.running then
    reg:start()
  end
  self.registrations[reg] = true
  reg.fin = lk.Finalizer(function()
    self.registrations[reg] = nil
  end)
end

--=============================================== PROTECTED
--                                                methods called from C++

function lib:failure(err)
  sched:log('error', err)
end

function lib:start()
  -- Server is ready, register our services.
  if not self.running then
    if self.registrations then
      -- In case this is called before we finished setting up
      -- mdns.Context in 'new'.
      for reg, _ in pairs(self.registrations) do
        local ok, err = pcall(reg.start, reg)
        if not ok then
          sched:log('error', err)
        end
      end
    end
    self.running = true
  end
end

function lib:stop(reg)
  -- Server not ready (offline, collision, etc).
  if self.running then
    for reg, _ in pairs(self.registrations) do
      local ok, err = pcall(reg.stop, reg)
      if not ok then
        sched:log('error', err)
      end
    end
    self.running = false
  end
end


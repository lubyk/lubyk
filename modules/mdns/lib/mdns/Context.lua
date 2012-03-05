--[[------------------------------------------------------

  mdns.Context
  ------------

  This class is used internally.

--]]------------------------------------------------------
local lib = mdns.core.Context
-- Hide internals
local WeakValue = {__mode = 'v'}

function mdns.Context()
  local self = lib.new()
  self.callbacks = {}
  self.registrations = setmetatable({}, WeakValue)
  if Lubyk.plat == 'macosx' then
    -- On macosx, we can always start the registration right away.
    self.running = true
  else
    self.running = false
  end
  return self
end

function lib:addSelectCallback(clbk)
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
  table.insert(self.registrations, reg)
end

--=============================================== Used by avahi
-- Not used on macosx.

function lib:failure(err)
  sched:log('error', err)
end

function lib:start()
  -- Server is ready, register our services.
  if not self.running then
    for _, reg in pairs(self.registrations) do
      local ok, err = pcall(reg.start, reg)
      if not ok then
        sched:log('error', err)
      end
    end
    self.running = true
  end
end

function lib:stop(reg)
  -- Server not ready (offline, collision, etc).
  if self.running then
    for _, reg in pairs(self.registrations) do
      local ok, err = pcall(reg.stop, reg)
      if not ok then
        sched:log('error', err)
      end
    end
    self.running = false
  end
end



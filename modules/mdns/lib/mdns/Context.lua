--[[------------------------------------------------------

  mdns.Context
  ------------

  This class is used internally.

--]]------------------------------------------------------
local lib = mdns.core.Context
-- Hide internals

function mdns.Context()
  local self = lib.new()
  self.callbacks = {}
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

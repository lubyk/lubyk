--[[------------------------------------------------------

  zmq.Poller
  ----------

  Poll for events for the registered zmq.Socket or 
  filedescriptors.

--]]------------------------------------------------------
zmq.Poller = zmq.Poller_core
local lib = zmq.Poller


-- This is called from lk.Scheduler loop in case the poller needs time to
-- run (for example for GUI stuff). This is not the case with this poller
-- where all events are handled by the poller itself so this function does
-- nothing.
function lib:resumeAt(at)
end

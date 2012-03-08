--[[------------------------------------------------------

  zmq.Poller
  ----------

  Poll for events for the registered zmq.Socket or 
  filedescriptors.

--]]------------------------------------------------------
require 'zmq.core'
zmq.Poller = zmq.Poller_core

--[[------------------------------------------------------

  zmq.Context
  -----------

  @internal Singleton used as base for all zmq.Socket
  creation.

--]]------------------------------------------------------
local lib = zmq.Context_core
zmq.Context = lib

-- Singleton
local ctx
local new = lib.new
function lib.new(...)
  if not ctx then
    ctx = new(...)
  end
  return ctx
end

--[[------------------------------------------------------

  zmq.Context
  -----------

  @internal Singleton used as base for all zmq.Socket
  creation.

--]]------------------------------------------------------
require 'zmq.core'
local lib = zmq.Context_core
zmq.Context = lib

-- Singleton
local ctx
local constr = lib.new
function lib.new(...)
  if not ctx then
    ctx = contr(...)
  end
  return ctx
end

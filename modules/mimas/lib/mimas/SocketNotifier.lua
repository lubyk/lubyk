--[[------------------------------------------------------

  mimas.SocketNotifier
  --------------------

  Used internally to process zmq and other sockets from
  within mimas event loop.

--]]------------------------------------------------------
local constr               = mimas_core.SocketNotifier
local mt                   = mimas_core.SocketNotifier_
mimas.SocketNotifier_      = mt
mimas.SocketNotifier       = constr
mimas.SocketNotifier_const = mimas_core.SocketNotifier_const

function mimas.SocketNotifier(...)
  return mimas.bootstrap('SocketNotifier', constr, ...)
end


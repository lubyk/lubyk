--[[------------------------------------------------------

  rk.TCPSocket
  ------------

  Streaming BSD Socket.

--]]------------------------------------------------------
require 'rk'
require 'rk.Socket'

function rk.TCPSocket(func)
  return rk.Socket(rk.Socket_const.TCP, func)
end

--[[------------------------------------------------------

  rk.UDPSocket
  ------------

  Datagram BSD Socket.

--]]------------------------------------------------------
require 'rk'
require 'rk.Socket'

function rk.UDPSocket(func)
  return rk.Socket(rk.Socket_const.UDP, func)
end

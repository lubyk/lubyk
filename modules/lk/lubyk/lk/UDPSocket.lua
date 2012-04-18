--[[------------------------------------------------------

  lk.UDPSocket
  ------------

  Datagram BSD Socket.

--]]------------------------------------------------------
require 'lk'
require 'lk.Socket'

function lk.UDPSocket(func)
  return lk.Socket(lk.Socket_const.UDP, func)
end

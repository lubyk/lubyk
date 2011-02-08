--[[------------------------------------------------------

  lk.TCPSocket
  ------------

  Streaming BSD Socket.

--]]------------------------------------------------------
require 'lk'
require 'lk.Socket'

function lk.TCPSocket(func)
  return lk.Socket(lk.Socket_const.TCP, func)
end

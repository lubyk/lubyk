--[[------------------------------------------------------

  socket
  ------

  LuaSocket Copyright © 2004-2007 Diego Nehab

--]]------------------------------------------------------
require 'socket.socket'
socket.prefix = 'socket'
setmetatable(socket, Autoload)

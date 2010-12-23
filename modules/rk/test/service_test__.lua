--[[------------------------------------------------------

  rk.Service
  -----------

  zmq + mDNS

--]]------------------------------------------------------
require 'rubyk'

local function receive_message(self, message)
  print(string.format("%s  <-- '%s' --", self.name, message))
end

mars = rk.Service("Mars", "_rubyk._tcp", receive_message)

mars:connect("Venus")

venus = rk.Service("Venus", "_rubyk._tcp", receive_message)

venus:connect("Mars")

local send = mars.send
function mars:send(message)
  print(string.format("Mars  --- '%s' --> ", message))
  send(self, message)
end

local send = venus.send
function venus:send(message)
  print(string.format("Venus --- '%s' --> ", message))
  send(self, message)
end

mars:send("Who are you ?")
worker:sleep(10)
venus:send("Your boss !")
worker:sleep(10)

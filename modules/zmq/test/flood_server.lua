require 'rubyk'

flooder = zmq.Pub()
print(flooder:port())

i = 0
while true do
  i = i + 1
  flooder:send(i, math.random())
end

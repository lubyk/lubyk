require 'rubyk'

client = zmq.Sub()
client:connect(string.format('tcp://localhost:%i', arg[1]))
i = 0
while true do
  i = i + 1
  print(client:recv())
end

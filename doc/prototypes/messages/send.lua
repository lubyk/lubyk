require 'msgpack'
require 'zmq'

local context = zmq.init(1)

-- set server
local server = context:socket(zmq.PUB)
server:bind('tcp://*:5556')

local i = 0
while true do
  i = (i + 1) % 20
  --print('zzz...')
  --os.execute('sleep 1')
  local url = '/a/b/c'
  --print('sending', url, i)
  server:send(msgpack.pack(url, i))

  url = '/a/b/d'
  --print('sending', url, i)
  server:send(msgpack.pack(url, i))
end

print('done')
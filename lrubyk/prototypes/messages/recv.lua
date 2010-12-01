require 'msgpack'
require 'zmq'

local context = zmq.init(1)

-- set client
local client = context:socket(zmq.SUB)
client:connect('tcp://localhost:5556')
-- get all values
client:subscribe()


local registrations = {}

function register(url, obj, method)
  registrations[url] = function(val)
    method(obj, val)
  end
end

Object = {}
obj = {name='hello'}

function obj:foo(val)
  print(string.format('[%s] <-- %i', self.name, val))
end

register('/a/b/c', obj, obj.foo)

-- should receive
while true do
  local url, i = msgpack.unpack(client:recv())
  local action = registrations[url]
  if action then
    action(i)
  else
    -- invalid url
    print('invalid url', url)
  end
end
--[[------------------------------------------------------

  zmq
  ---

  ZeroMQ (0MQ) networking library bindings.

--]]------------------------------------------------------
zmq = Autoload('zmq')
-- fast pack/unpack directly in zmq
require 'msgpack.vendor'
require 'zmq.vendor'
require 'zmq.constants'

--[[------------------------------------------------------

  zmq
  ---

  ZeroMQ (0MQ) networking library bindings.

--]]------------------------------------------------------
zmq = Autoload('zmq')
-- fast pack/unpack directly in zmq
require 'msgpack.vendor'
require 'zmq.vendor'
-- FIXME: Should this be set to 1 ?
zmq.ctx = zmq.init(1)

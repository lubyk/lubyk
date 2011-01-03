--[[------------------------------------------------------

  rk.Socket
  ---------

  The socket is a simple wrapper around BSD sockets.

--]]------------------------------------------------------
require 'rubyk'

local should = test.Suite('rk.Socket')
local counter = 0

function should.bind_to_random_port()
  local socket = rk.Socket()
  local port = socket:bind()

  assert_true(port < 65535)
  assert_true(port > 1024)
  assert_equal(port, socket:port())

  local socket2 = rk.Socket()
  local port2 = socket2:bind()
  assert_true(port2 < 65535)
  assert_true(port2 > 1024)
  assert_equal(port2, socket2:port())
  assert_not_equal(port, port2)
end

function should.convert_to_string()
  assert_equal('string', type(rk.Socket():__tostring()))
end

function should.create_many_sockets()
  local sock_list = {}
  for i = 1,100 do
    sock_list[i] = rk.Socket()
    sock_list[i]:bind() -- creates socket fd + bind
  end
  -- should just work
  assert_equal('userdata', type(sock_list[100]))
end

function should.connect()
  local socket = rk.Socket()
  socket:connect("rubyk.org", 80)
  assert_match('--> rubyk.org:80', socket:__tostring())
end

function should.listen_and_accept()
  local socket = rk.Socket()
  local port   = socket:bind()
  local continue = false
  socket:listen()
  rk.Thread(function()
    local sock2 = socket:accept()
    continue = true
    -- should bind to different port
    assert_not_equal(port, sock2:remote_port())
  end)

  worker:sleep(20) -- start thread

  -- I can now connect to this remote
  local socket2 = rk.Socket()
  socket2:connect("localhost", port)
  while not continue do
    worker:sleep(20)
  end
end

test.all()
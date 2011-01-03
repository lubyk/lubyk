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
  socket:connect('rubyk.org', 80)
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
  socket2:connect('localhost', port)
  while not continue do
    worker:sleep(20)
  end
end

function should.send_and_recv()
  local server = rk.Socket()
  local port   = server:bind()
  server:listen()
  local client = rk.Socket()
print("connecting")
  client:connect('localhost', port)
  local acc = server:accept()
print("sending")
  client:send(1, 2, "hello")
print("send done")
  received = {acc:recv()}
  assert_equal({1, 2, "hello"}, received)
end

function should.send_and_receive()
  local server   = rk.Socket()
  local port     = server:bind()
  server:listen()

  local client = rk.Socket()

  client:connect('localhost', port)

  local sender = server:accept()

  local continue = false
  local received = nil
  local thread
  thread = rk.Thread(function()
    while thread:should_run() do
      continue = true
      received = {client:recv()}
    end
  end)

  worker:sleep(20)

  local function send_and_receive(...)
    continue = false
    -- cannot use ... or arg is set to nil
    sender:send(unpack(arg))
    while not continue do
      worker:sleep(10)
    end

    for i, v in ipairs(arg) do
      assert_equal(arg[i], received[i])
    end
  end

  -- string
  send_and_receive("Hello Rubyk!")
  -- number, nil, bool
  send_and_receive(1.234567)
  send_and_receive(nil)
  send_and_receive(true)
  send_and_receive(false)
  -- array
  -- we cannot send an array like this
  -- send_and_receive({1, 2, 3}) ===> received as (1, 2, 3) (not {1, 2, 3})
  send_and_receive(1, {1, 2, 3})

  send_and_receive(1, {1, 2, {4, 5}})

  send_and_receive("1", {1, "zombie", {4.44, 5}})

  -- hash
  send_and_receive("/one/two", {one=2, three="four"})

  -- multi values
  send_and_receive(1,2,3)
  send_and_receive("/amp/gain", 3.5)
  send_and_receive("/amp/gain", {1, 2, {"foo", "bar", 5}})

  receiver:kill()
end

test.all()
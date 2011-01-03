--[[------------------------------------------------------

  rk.TCPSocket
  ------------

  The TCP socket is a simple wrapper around a Streaming
  BSD socket.

--]]------------------------------------------------------
require 'rubyk'

local should = test.Suite('rk.Socket')
local counter = 0

--function should.bind_to_random_port()
--  local socket = rk.TCPSocket()
--  local port = socket:bind()
--
--  assert_true(port < 65535)
--  assert_true(port > 1024)
--  assert_equal(port, socket:port())
--
--  local socket2 = rk.TCPSocket()
--  local port2 = socket2:bind()
--  assert_true(port2 < 65535)
--  assert_true(port2 > 1024)
--  assert_equal(port2, socket2:port())
--  assert_not_equal(port, port2)
--end
--
--function should.convert_to_string()
--  assert_equal('string', type(rk.TCPSocket():__tostring()))
--end
--
--function should.create_many_sockets()
--  local sock_list = {}
--  for i = 1,100 do
--    sock_list[i] = rk.TCPSocket()
--    sock_list[i]:bind() -- creates socket fd + bind
--  end
--  -- should just work
--  assert_equal('userdata', type(sock_list[100]))
--end
--
--function should.connect()
--  local socket = rk.TCPSocket()
--  socket:connect('rubyk.org', 80)
--  assert_match('--> rubyk.org:80', socket:__tostring())
--end
--
--function should.listen_and_accept()
--  local socket = rk.TCPSocket()
--  local port   = socket:bind()
--  local continue = false
--  socket:listen()
--  rk.Thread(function()
--    local sock2 = socket:accept()
--    continue = true
--    -- should bind to different port
--    assert_not_equal(port, sock2:remote_port())
--  end)
--
--  worker:sleep(20) -- start thread
--
--  -- I can now connect to this remote
--  local socket2 = rk.TCPSocket()
--  socket2:connect('localhost', port)
--  while not continue do
--    worker:sleep(20)
--  end
--end
--
function should.send_and_recv()
  local port
  local server = rk.TCPSocket(function(server)
    port = server:bind()
    server:listen()
    local acc = server:accept()
    print('accept-->', acc)
    acc:send(1, 2, "hello")
    print("send done")
    --acc:send(5)
  end)
  worker:sleep(20) -- start server

  local client = rk.TCPSocket()
  print("connecting")
  client:connect('localhost', port)
  print('client', client)
  local received = {client:recv()}

  assert_equal({1, 2, "hello"}, received)

  received = {client:recv()}

  assert_equal({5}, received)
end

--function should.send_and_receive()
--  local port
--  local continue = false
--  local received = {}
--  local server = rk.TCPSocket(function(server)
--    port = server:bind()
--    server:listen()
--    local acc = server:accept()
--    acc:send(true) -- tell client that connection is OK
--
--    while received do
--      -- keep receiving
--      received = {acc:recv()}
--      print('<---', unpack(received))
--      continue = true
--    end
--  end)
--  worker:sleep(20) -- start server
--
--  local client = rk.TCPSocket()
--  client:connect('localhost', port)
--  -- wait for accept
--  client:recv()
--
--  local function send_and_receive(...)
--    continue = false
--    -- cannot use ... or arg is set to nil
--    client:send(unpack(arg))
--    while not continue do
--      worker:sleep(10)
--    end
--
--    for i, v in ipairs(arg) do
--      assert_equal(arg, received)
--    end
--  end
--
--  -- string
--  send_and_receive("Hello Rubyk!")
--  -- number, nil, bool
--  send_and_receive(1.234567)
--  send_and_receive(nil)
--  send_and_receive(true)
--  send_and_receive(false)
--  -- array
--  -- we cannot send an array like this
--  -- send_and_receive({1, 2, 3}) ===> received as (1, 2, 3) (not {1, 2, 3})
--  send_and_receive(1, {1, 2, 3})
--
--  send_and_receive(1, {1, 2, {4, 5}})
--
--  send_and_receive("1", {1, "zombie", {4.44, 5}})
--
--  -- hash
--  send_and_receive("/one/two", {one=2, three="four"})
--
--  -- multi values
--  send_and_receive(1,2,3)
--  send_and_receive("/amp/gain", 3.5)
--  send_and_receive("/amp/gain", {1, 2, {"foo", "bar", 5}})
--  send_and_receive(nil)
--end

test.all()
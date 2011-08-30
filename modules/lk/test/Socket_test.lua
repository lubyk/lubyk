--[[------------------------------------------------------

  lk.Socket test
  --------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.Socket')

function should.autoLoad()
  assertTrue(lk.Socket)
end

function should.runMutlithreaded(t)
  t.sequence = {}
  function log(msg)
    table.insert(t.sequence, msg)
  end
  t.server = lk.Socket()
  t.server:bind('*', 0)
  t.server:listen()
  t.port = t.server:port()
  -- run server in new thread
  t.thread = lk.Thread(function()
    -- will start as soon as we release the mutex
    log('srv:thread')
    local client = t.server:accept()
    log('srv:accept')
    t.received = client:recv()
    log('srvi:recv')
    client:close()
    log('srvi:close')
  end)
  t.client = lk.Socket()
  t.client:connect('127.0.0.1', t.port)
  log('cli:connect')
  -- 'send' does not unlock mutex
  t.client:send('Hello Lubyk!\n')
  log('cli:send')
  -- The server thread finally has some time to run
  t.thread:join()
  -- should give us control back on 'accept'
  assertValueEqual('Hello Lubyk!', t.received)
  assertValueEqual({
    'cli:connect',
    'cli:send',
    'srv:thread',
    'srv:accept',
    'srvi:recv',
    'srvi:close',
  }, t.sequence)
end

function should.send(t)
  t.server = lk.Socket()
  t.server:bind('*', 0)
  t.server:listen()
  t.port = t.server:port()
  -- run server in new thread
  t.thread = lk.Thread(function()
    -- will start as soon as we release the mutex
    t.srv_client = t.server:accept()
    t.received1 = t.srv_client:recv()
    t.received2 = t.srv_client:recv()
    t.srv_client:close()
  end)
  t.client = lk.Socket()
  t.client:connect('127.0.0.1', t.port)
  t.client:send('Hello Lubyk!\nOne two\n')

  -- The server thread finally has some time to run
  t.thread:join()
  t.client:close()
  -- should give us control back on 'accept'
  assertEqual('Hello Lubyk!', t.received1)
  assertEqual('One two', t.received2)
end

function should.recvBytes(t)
  t.server = lk.Socket()
  t.server:bind('*', 0)
  t.server:listen()
  t.port = t.server:port()
  -- run server in new thread
  t.thread = lk.Thread(function()
    -- will start as soon as we release the mutex
    local client = t.server:accept()
    t.received1 = client:recv(6)
    t.received2 = client:recv(4)
    client:close()
  end)
  t.client = lk.Socket()
  t.client:connect('127.0.0.1', t.port)
  -- 'send' does not unlock mutex
  t.client:send('0123456789')
  -- The server thread finally has some time to run
  t.thread:join()
  -- should give us control back on 'accept'
  assertValueEqual('012345', t.received1)
  assertValueEqual('6789', t.received2)
end

function should.sendMessages(t)
  t.server = lk.Socket()
  t.server:bind('*', 0)
  t.server:listen()
  t.port = t.server:port()
  -- run server in new thread
  t.thread = lk.Thread(function()
    -- will start as soon as we release the mutex
    local client = t.server:accept()
    t.received1 = client:recvMsg()
    t.received2, t.received3 = client:recvMsg()
    client:close()
  end)
  t.client = lk.Socket()
  t.client:connect('127.0.0.1', t.port)
  -- 'send' does not unlock mutex
  t.client:sendMsg('0123456789')
  t.client:sendMsg(34, {a = 'blah'})
  -- The server thread finally has some time to run
  t.thread:join()
  -- should give us control back on 'accept'
  assertEqual('0123456789', t.received1)
  assertEqual(34, t.received2)
  assertValueEqual({a='blah'}, t.received3)
end

-- send send/recv with sendMsg/recvMsg
function should.sendMessagesWithPartialBuffer(t)
  t.server = lk.Socket()
  t.server:bind('*', 0)
  t.server:listen()
  t.port = t.server:port()
  -- run server in new thread
  t.thread = lk.Thread(function()
    -- will start as soon as we release the mutex
    local client = t.server:accept()
    t.received1 = client:recv('*l')
    t.received2, t.received3 = client:recvMsg()
    t.received4 = client:recv('*l')
    t.received5 = client:recvMsg()
    client:close()
  end)
  t.client = lk.Socket()
  t.client:connect('127.0.0.1', t.port)
  -- 'send' does not unlock mutex
  t.client:send('abcd\n')
  t.client:sendMsg(34, {a = 'blah'})
  t.client:send('Hello World!\n')
  t.client:sendMsg({a=1,b=2})
  -- The server thread finally has some time to run
  t.thread:join()
  -- should give us control back on 'accept'
  assertEqual('abcd', t.received1)
  assertEqual(34, t.received2)
  assertValueEqual({a='blah'}, t.received3)
  assertEqual('Hello World!', t.received4)
  assertValueEqual({a=1,b=2}, t.received5)
end

-- FIXME: test Socket::recv('*a')

--function should.bind_to_random_port()
--  local socket = lk.TCPSocket()
--  local port = socket:bind()
--
--  assertTrue(port < 65535)
--  assertTrue(port > 1024)
--  assertEqual(port, socket:port())
--
--  local socket2 = lk.TCPSocket()
--  local port2 = socket2:bind()
--  assertTrue(port2 < 65535)
--  assertTrue(port2 > 1024)
--  assertEqual(port2, socket2:port())
--  assert_not_equal(port, port2)
--end
--
--function should.convert_to_string()
--  assertEqual('string', type(lk.TCPSocket():__tostring()))
--end
--
--function should.create_many_sockets()
--  local sock_list = {}
--  for i = 1,100 do
--    sock_list[i] = lk.TCPSocket()
--    sock_list[i]:bind() -- creates socket fd + bind
--  end
--  -- should just work
--  assertEqual('userdata', type(sock_list[100]))
--end
--
--function should.connect()
--  local socket = lk.TCPSocket()
--  socket:connect('lubyk.org', 80)
--  assertMatch('--> lubyk.org:80', socket:__tostring())
--end
--
--function should.listen_and_accept()
--  local socket = lk.TCPSocket()
--  local port   = socket:bind()
--  local continue = false
--  socket:listen()
--  lk.Thread(function()
--    local sock2 = socket:accept()
--    continue = true
--    -- should bind to different port
--    assert_not_equal(port, sock2:remote_port())
--  end)
--
--  sleep(20) -- start thread
--
--  -- I can now connect to this remote
--  local socket2 = lk.TCPSocket()
--  socket2:connect('localhost', port)
--  while not continue do
--    sleep(20)
--  end
--end
--

test.all()

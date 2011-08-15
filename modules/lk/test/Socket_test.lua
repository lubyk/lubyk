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
  t.server:bind("*", 0)
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
  t.client:send("Hello Lubyk!")
  log('cli:send')
  -- The server thread finally has some time to run
  t.thread:join()
  -- should give us control back on 'accept'
  assertValueEqual("Hello Lubyk!", t.received)
  assertValueEqual({
    'cli:connect',
    'cli:send',
    'srv:thread',
    'srv:accept',
    'srvi:recv',
    'srvi:close',
  }, t.sequence)
end

function should.rawSend(t)
  t.server = lk.Socket()
  t.server:bind("*", 0)
  t.server:listen()
  t.port = t.server:port()
  -- run server in new thread
  t.thread = lk.Thread(function()
    -- will start as soon as we release the mutex
    t.srv_client = t.server:accept()
    t.received1 = t.srv_client:rawRecv()
    t.received2 = t.srv_client:rawRecv()
    t.srv_client:close()
  end)
  t.client = lk.Socket()
  t.client:connect('127.0.0.1', t.port)
  t.client:rawSend("Hello Lubyk!\nOne two\n")

  -- The server thread finally has some time to run
  t.thread:join()
  t.client:close()
  -- should give us control back on 'accept'
  assertEqual("Hello Lubyk!", t.received1)
  assertEqual("One two", t.received2)
end

function should.rawRecvBytes(t)
  t.server = lk.Socket()
  t.server:bind("*", 0)
  t.server:listen()
  t.port = t.server:port()
  -- run server in new thread
  t.thread = lk.Thread(function()
    -- will start as soon as we release the mutex
    local client = t.server:accept()
    t.received1 = client:rawRecv(6)
    t.received2 = client:rawRecv(4)
    client:close()
  end)
  t.client = lk.Socket()
  t.client:connect('127.0.0.1', t.port)
  -- 'send' does not unlock mutex
  t.client:rawSend("0123456789")
  -- The server thread finally has some time to run
  t.thread:join()
  -- should give us control back on 'accept'
  assertValueEqual("012345", t.received1)
  assertValueEqual("6789", t.received2)
end

-- TODO: test Socket::rawRecv('*a')

test.all()

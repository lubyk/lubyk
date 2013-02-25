--[[------------------------------------------------------

  lk.ProcessWatch test
  --------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.ProcessWatch')

function mockDelegate(t)
  return {
    last_op = nil,
    last_name = nil,
    processConnected = function(self, process)
      self.last_op = 'connected'
      self.last_name = process.name
      t.continue = true
    end,
    processDisconnected = function(self, process)
      self.last_op = 'disconnected'
      self.last_name = process.name
      t.continue = true
    end
  }
end

function should.returnRemoteProcessOnProcess()
  local pw = lk.ProcessWatch()
  local p = pw:process('hello')
  assertEqual('lk.RemoteProcess', p.type)
  assertFalse(p.online)
end

function should.callProcessConnected(t)
  local del = mockDelegate(t)
  local pw = lk.ProcessWatch():addDelegate(del)
  local p = pw:process('hello')
  assertFalse(p.online)
  -- get processConnected stuff
  sleep(0.2)
  t.continue = false
  -- register 'hello'
  local s = lk.Service(Lubyk.zone .. ':hello')
  t:timeout(3, function()
    return t.continue
  end)
  assertTrue(t.continue)
  assertEqual('connected', del.last_op)
  assertEqual('hello', del.last_name)
  assertTrue(p.online)
end

test.all()

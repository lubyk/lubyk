--[[------------------------------------------------------

  lk.Node test
  ------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.ProcessWatch')

function mockDelegate(t)
  return {
    last_op = nil,
    last_name = nil,
    processConnected = function(self, name)
      self.last_op = 'connected'
      self.last_name = name
      t.continue = true
    end,
    processDisconnected = function(self, name)
      self.last_op = 'disconnected'
      self.last_name = name
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
  -- register 'hello'
  local s = lk.Service(Lubyk.zone .. ':hello')
  t:timeout(function(done)
    if done or t.continue then
      assertTrue(t.continue)
      assertEqual('connected', del.last_op)
      assertEqual('hello', del.last_name)
      assertTrue(p.online)
      return true
    end
  end)
end

test.all()
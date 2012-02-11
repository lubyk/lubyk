--[[------------------------------------------------------

  _control.lk.Slider test
  -----------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('_control.lk.Slider')
local withUser = should:testWithUser()

function should.autoload()
  assertType('table', _control.lk.Slider)
end

local Slider = _control.lk.Slider

function should.createConnectors()
  local s = Slider('foo')
  local x = s.conn_s
  assertEqual('editor.Connector', x.type)
end

function should.findConnector()
  local s = Slider('foo')
  local x = s:connector('x')
  assertEqual('editor.Connector', x.type)
end

function withUser.should.showSlider(t)
  local s = Slider('foo')
  s.s = 0.8
  s.remote_s = 0.5
  s:show()
  t:timeout(function()
    return s.s == 0
  end)
  assertEqual(0, s.s)
end



test.all()


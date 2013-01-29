--[[------------------------------------------------------

  lk.ParamMethod test
  -------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.ParamMethod')

local function mockNode()
  return {
    outlets = {},
    slots = {
      outlets = {},
    },
    env = {},
    url = function() return '/a/b' end,
    declareParams = function(self, l) self.def = l end,
  }
end

function should.createParamMethod()
  local node = mockNode()
  local p = lk.ParamMethod(node)
  assertType('table', p)
  assertEqual('lk.ParamMethod', p.type)
end

function should.declareParameters()
  local node = mockNode()
  local p = lk.ParamMethod(node)
  local list = {
    x = 0,
    y = 1,
  }
  assertPass(function()
    p(list)
  end)
  assertEqual(list, node.def)
end

test.all()


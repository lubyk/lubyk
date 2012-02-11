--[[------------------------------------------------------

  editor.Connector test
  ---------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('editor.Connector')

function should.autoload()
  assertType('table', editor.Connector)
end

function should.setType()
  local conn = editor.Connector({}, 'x')
  assertEqual('editor.Connector', conn.type)
end

function should.callbackCtrlOnChanged()
  local ctrl = {}
  local res
  function ctrl:changed(...)
    res = {...}
  end
  local conn = editor.Connector(ctrl, 'x')
  -- ! No 'self' here.
  conn.changed(4.55)
  assertValueEqual({
    'x',
    4.55,
  }, res)
end

function should.connect()
  local ctrl = {}
  local process = {}
  local url = '/foo/a/b/_/foo'
  local conn = editor.Connector(ctrl, 'x')
  conn:connect(process, url)
end

function should.pushchange()
  local ctrl = {}
  local process = {online = true}
  process.push = process
  local res
  function process:send(url, val)
    res = val
  end
  local url = '/foo/a/b/_/foo'
  local conn = editor.Connector(ctrl, 'x')
  conn:connect(process, url)
  conn.change(12.33)
  assertValueEqual({
    nodes = {
      a = {
        b = {
          _ = {
            foo = 12.33,
          },
        },
      },
    },
  }, res)
end


test.all()


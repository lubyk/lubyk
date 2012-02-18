--[[------------------------------------------------------

  editor.LogView test
  -------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('editor.LogView')
local withUser = should:testWithUser()

function withUser.should.displayWindow(t)
  local l = editor.LogView()
  l:addMessage('/a/bob', 'error', [[
stdin:1: attempt to index global 'yaml' (a nil value)
stack traceback:
	stdin:1: in main chunk
	[C]: ?
  Blah blah
]])
  l:addMessage('/a/bob', 'info', 'hello world')
  l:addMessage('/a/bob', 'warn', 'warning')
  l:show()

  function l:closed()
    t.continue = true
  end
  
  t:timeout(10000, function()
    return t.continue
  end)
end


test.all()


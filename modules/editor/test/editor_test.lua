--[[------------------------------------------------------

  editor test
  -----------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('editor')

function should.auto_load()
  assertTrue(editor)
end

test.all()

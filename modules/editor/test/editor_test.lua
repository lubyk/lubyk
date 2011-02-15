--[[------------------------------------------------------

  editor test
  -----------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('editor')

function should.auto_load()
  assert_true(editor)
end

test.all()

--[[------------------------------------------------------

editor.Control test
-------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('editor.Control')

function should.autoload()
  assertType('table', editor.Control)
end

test.all()



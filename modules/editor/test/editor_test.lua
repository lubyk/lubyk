--[[------------------------------------------------------

  editor test
  -----------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('editor')

function should.autoLoad()
  assertTrue(editor)
end

function should.createMain()
  assertEqual('editor.Zone', editor.main.type)
end

function should.shouldEditFile()
  assertPass(function()
    editor.main:editFile(fixture.path('simple.yml'))
  end)
end
test.all()

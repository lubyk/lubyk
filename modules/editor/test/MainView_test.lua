--[[------------------------------------------------------

  editor.MainView test
  --------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('editor.MainView')

function should.drawMainView(t)
  t.remote   = lk.Process(fixture.path('simple.yml'))
  t.editor   = editor.Main()
  t.watch    = editor.ProcessWatch(t.editor)
  t.editor:setView(editor.MainView())
  t.editor.main_view:show()
  sleep(2000)
end

test.gui()

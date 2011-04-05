--[[------------------------------------------------------

  editor.LibraryView test
  -----------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('editor.LibraryView')

function should.drawLibraryView(t)
  t.view = editor.LibraryView()
  t.view:show()

  t.thread = lk.Thread(function()
    sleep(5000)
    t.view:close()
    assertTrue(true)
  end)
end

test.gui()

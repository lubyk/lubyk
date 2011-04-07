--[[------------------------------------------------------

  editor.LibraryView test
  -----------------------

  ...

--]]------------------------------------------------------
require 'lubyk'
local should = test.Suite('editor.LibraryView')
local data = editor.Library(sqlite3.open_memory())
data.sources = { test = fixture.path('nodes') }
data:sync()

function should.drawLibraryView(t)
  t.view = editor.LibraryView(data)
  t.view:show()
  t.thread = lk.Thread(function()
    sleep(2000)
    t.view:close()
    assertTrue(true)
  end)
end

test.gui()


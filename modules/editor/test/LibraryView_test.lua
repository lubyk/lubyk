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
  t.view:move(10,10)
  t.view:show()
  t:timeout(function(done)
    return done or t.continue
  end)
  assertTrue(t.continue)
  t.view:close()
end

test.all()

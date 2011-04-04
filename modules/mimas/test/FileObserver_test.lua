--[[------------------------------------------------------

  mimas.FileObserver
  ------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('mimas.FileObserver')

function should.autoLoad()
  assertTrue(mimas.FileObserver)
end

function should.observeFiles(t)
  local filepath = 'foo/bar/baz.txt'
  local continue = false

  lk.writeall(filepath, 'Hello')
  t.observer = mimas.FileObserver(filepath)

  function t.observer.pathChanged(path)
    print(path, 'changed')
    if path == filepath then
      assertTrue(true)
      t.observer:removePath(filepath)
      lk.rmTree('foo')
    end
  end

  os.execute(string.format("%s '%s'", 'mate', filepath))
end

test.gui()
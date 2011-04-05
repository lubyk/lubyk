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
  -- DUMMY WINDOW TO STOP TEST ON CLOSE
  t.win = mimas.Window()
  t.label = mimas.Label('Please edit file')
  t.win:addWidget(t.label)
  t.win:show()
  t.win:move(100,100)
  t.win:resize(100,100)
  ---------------------------------------

  -- adding path twice should not raise
  assertPass(function()
    t.observer:addPath(filepath)
  end)

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
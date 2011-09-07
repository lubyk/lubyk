require 'lubyk'

local should = test.Suite('lk.path helpers')

function should.readall()
  assertEqual('Hello Lubyk!', lk.readall(fixture.path('io.txt')))
end

function should.changeDir()
  local current = lk.dir()
  lk.withDirectory(current .. '/fixtures', function()
    assertMatch(current .. '/fixtures$', lfs.currentdir())
  end)
end

function should.absolutizePath()
  assertEqual(lfs.currentdir() .. '/foo/bar', lk.absolutizePath('foo/bar'))
  assertEqual('/foo/bar', lk.absolutizePath('/foo/bar'))
  -- assertEqual('/One/two/foo/bar', lk.absolutizePath('foo/bar', '/One/two'))
  -- assertEqual('/foo/bar', lk.absolutizePath('/foo/bar', '/One/two'))
end

function should.makePath()
  local path = fixture.path('tmp/foo/bar/baz')
  lk.rmTree(fixture.path('tmp'), true)
  assertPass(function()
    lk.makePath(path)
  end)
  assertEqual('directory', lk.fileType(fixture.path('tmp/foo')))
  assertEqual('directory', lk.fileType(fixture.path('tmp/foo/bar')))
  assertEqual('directory', lk.fileType(fixture.path('tmp/foo/bar/baz')))
  lk.rmTree(fixture.path('tmp'), true)
end

function should.notRmTreeRecursive()
  local path = fixture.path('tmp/fo"o/bar/baz')
  assertPass(function()
    lk.makePath(path)
  end)
  assertEqual('directory', lk.fileType(fixture.path('tmp/fo"o')))
  assertEqual('directory', lk.fileType(fixture.path('tmp/fo"o/bar')))
  assertEqual('directory', lk.fileType(fixture.path('tmp/fo"o/bar/baz')))
  assertFalse(lk.rmTree(fixture.path('tmp/fo"o')))
  assertTrue(lk.exist(fixture.path('tmp/fo"o')))
  lk.rmTree(fixture.path('tmp'), true)
end

function should.rmTree()
  local path = fixture.path('tmp/fo"o/bar/baz')
  assertPass(function()
    lk.makePath(path)
  end)
  assertEqual('directory', lk.fileType(fixture.path('tmp/fo"o')))
  assertEqual('directory', lk.fileType(fixture.path('tmp/fo"o/bar')))
  assertEqual('directory', lk.fileType(fixture.path('tmp/fo"o/bar/baz')))
  lk.rmTree(fixture.path('tmp/fo"o'), true)
  assertFalse(lk.exist(fixture.path('tmp/fo"o')))
end

function should.rmFile()
  local path = fixture.path('tmp.txt')
  lk.writeall(path, 'Hello')
  assertTrue(lk.exist(path))
  assertPass(function()
    lk.rmFile(path)
    lk.rmFile(path)
  end)
  assertFalse(lk.exist(path))
end

function should.writeall()
  local foo = fixture.path('tmp/foo')
  lk.rmTree(foo, true)
  local tmp_path = foo .. '/bar/lk_test_writeall.txt'
  os.remove(tmp_path)
  lk.writeall(tmp_path, 'This is the message')
  assertEqual('This is the message', lk.readall(tmp_path))
  lk.rmTree(foo, true)
end

function should.change_dir_before_dofile()
  dofile(fixture.path('dofile.lua'))
  assertEqual(2, lk_test_dofile_val)
end

function should.split()
  local list = lk.split('cat,dog,mouse', ',')
  assertEqual('cat'  , list[1])
  assertEqual('dog'  , list[2])
  assertEqual('mouse', list[3])
  assertEqual(3, #list)
end

function should.splitChars()
  local list = lk.split('cat')
  assertEqual('c', list[1])
  assertEqual('a', list[2])
  assertEqual('t', list[3])
  assertEqual(3, #list)
end

function should.absToRel()
  assertEqual('play/in/trigger', lk.absToRel('/foo/play/in/trigger', '/foo'))
  assertEqual('/foo/bar', lk.absToRel('/foo/bar', '/foo/bar'))
  assertEqual('/foo/baz/boom', lk.absToRel('/foo/baz/boom', '/foo/bar'))
end

function should.returnEmptyOnSpitStartingWithSep()
  local list = lk.split('/my/home', '/')
  assertEqual(''    , list[1])
  assertEqual('my'  , list[2])
  assertEqual('home', list[3])
  assertEqual(3, #list)
end

function should.provideSource()
  assertMatch('modules/lk/test/lk_test.lua$', lk.source())
end

function should.provideDir()
  assertMatch('modules/lk/test$', lk.dir())
end

function should.provideFile()
  assertMatch('modules/lk/test/lk_test.lua$', lk.file())
end

function should.testFileExistence()
  assertEqual('file', lk.fileType(fixture.path('foo.yml')))
  assertEqual(nil, lk.fileType(fixture.path('complex.yml')))
  assertEqual('directory', lk.fileType(lk.dir()))
  assertEqual(nil, lk.fileType(nil))
end

function should.findCodeLocally()
  assertMatch('adagio', lk.findCode(fixture.path(), 'pass'))
end

function should.findCodeInLibs()
  assertMatch('Triggers regular bangs', lk.findCode(lk.dir(), 'lubyk.Metro'))
end

function should.shellQuote()
  assertEqual('"foo"', lk.shellQuote('foo'))
  -- foo 25"  --> "foo 25\""
  assertEqual('"foo 25\\\""', lk.shellQuote('foo 25"'))
  -- foo 25\" --> "foo 25\\\""
  assertEqual('"foo 25\\\\\\\""', lk.shellQuote('foo 25\\"'))
end

test.all()

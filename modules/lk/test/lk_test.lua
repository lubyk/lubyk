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
end

function should.makePath()
  local path = 'foo/bar/baz'
  lk.rmTree('foo')
  assertPass(function()
    lk.makePath('foo/bar/baz')
  end)
  assertEqual('directory', lk.fileType('foo'))
  assertEqual('directory', lk.fileType('foo/bar'))
  assertEqual('directory', lk.fileType('foo/bar/baz'))
  lk.rmTree('foo')
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
  lk.rmTree('foo')
  local tmp_path = 'foo/bar/lk_test_writeall.txt'
  os.remove(tmp_path)
  lk.writeall(tmp_path, 'This is the message')
  assertEqual('This is the message', lk.readall(tmp_path))
  lk.rmTree('foo')
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

test.all()

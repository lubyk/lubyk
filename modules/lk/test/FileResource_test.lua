--[[------------------------------------------------------

  lk.FileResource test
  --------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.FileResource')

function should.teardown()
  lk.rmFile(fixture.path('project/FileResource.tmp'))
  lk.rmFile(fixture.path('project/OneFileResource.tmp'))
  lk.rmTree(fixture.path('project/folder'))
end

function should.autoload()
  assertType('table', lk.FileResource)
end

function should.mapFile()
  local path = fixture.path('project/example.lkp')
  local rez = lk.FileResource(path, fixture.path())
  local old_date = os.time{year=2011,month=9,day=6}
  local new_date = os.time()
  assertEqual('lkp', rez.ext)
  assertEqual('example.lkp', rez.name)
  assertEqual('/project/example.lkp', rez.href)
  assertInRange(old_date, new_date, rez.getlastmodified)
  assertInRange(old_date, new_date, rez.creationdate)
  assertInRange(10, 1000, rez.getcontentlength)
  assertEqual('text/yaml', rez.contenttype)
end

function should.mapDirectory()
  local path = fixture.path('project')
  local rez = lk.FileResource(path, fixture.path())
  assertNil(rez.ext)
  assertEqual('project', rez.name)
  assertValueEqual({xml = 'collection'}, rez.resourcetype)
  assertNil(rez.getlastmodified)
  assertNil(rez.creationdate)
end

function should.listChildren()
  local path = lk.absolutizePath(fixture.path('project'))
  local rez = lk.FileResource(path)
  local children = rez:children()
  assertEqual(5, #children)
  assertEqual('bang', children[1].name)
  -- should propagate rootpath
  assertEqual(path, children[2].rootpath)
  assertEqual('example.lkp', children[2].name)
end

function should.delete()
  local path = fixture.path('project/FileResource.tmp')
  lk.writeall(path, 'one two')
  assertTrue(lk.exist(path))
  local rez = lk.FileResource(path)
  assertTrue(rez:delete())
  assertFalse(lk.exist(path))
end

function should.deleteChildResource()
  local path = fixture.path('project')
  local rez = lk.FileResource(path)
  local childpath = fixture.path('project/FileResource.tmp')
  lk.writeall(childpath, 'one two')
  assertTrue(lk.exist(path))
  -- create children cache
  local children = rez:children()
  assertEqual(6, #children)

  assertTrue(rez:deleteChild(lk.FileResource(childpath, rez.rootpath)))
  children = rez:children()
  assertEqual(5, #children)
  assertFalse(lk.exist(childpath))
end

function should.deleteChildName()
  local path = fixture.path('project')
  local rez = lk.FileResource(path)
  local childpath = fixture.path('project/FileResource.tmp')
  lk.writeall(childpath, 'one two')
  assertTrue(lk.exist(path))
  -- create children cache
  local children = rez:children()
  assertEqual(6, #children)

  assertTrue(rez:deleteChild('FileResource.tmp'))
  children = rez:children()
  assertEqual(5, #children)
  assertFalse(lk.exist(childpath))
end

function should.createChild()
  local path = fixture.path('project')
  local rez = lk.FileResource(path)
  -- create children cache
  local children = rez:children()
  assertEqual(5, #children)

  local childpath = fixture.path('project/FileResource.tmp')
  assertTrue(rez:createChild('FileResource.tmp', 'A new FileResource'))
  local child = lk.FileResource(childpath, path)
  assertEqual('FileResource.tmp', child.name)
  assertEqual('/FileResource.tmp', child.href)
  assertEqual('A new FileResource', lk.readall(fixture.path('project/FileResource.tmp')))
  assertEqual('A new FileResource', child:body())
  children = rez:children()
  assertEqual(6, #children)
end

function should.createChildDir()
  local path = fixture.path('project')
  local rez = lk.FileResource(path)
  -- create children cache
  local children = rez:children()
  assertEqual(5, #children)

  local childpath = fixture.path('project/folder')
  assertTrue(rez:createChild('folder'))
  local child = lk.FileResource(childpath, path)
  assertEqual('folder', child.name)
  assertEqual('/folder', child.href)
  assertTrue(child.is_dir)
  children = rez:children()
  assertEqual(6, #children)
end

function should.update()
  local path = fixture.path('project')
  local rez = lk.FileResource(path)
  local childpath = fixture.path('project/FileResource.tmp')
  assertTrue(rez:createChild('FileResource.tmp', 'A new FileResource'))
  local child = lk.FileResource(childpath, path)
  assertEqual('A new FileResource', child:body())

  assertTrue(child:update('Something else'))
  assertEqual('Something else', child:body())
  assertEqual('Something else', lk.readall(fixture.path('project/FileResource.tmp')))
end

function should.moveChild()
  local path = fixture.path('project')
  local rez = lk.FileResource(path)
  local childpath = fixture.path('project/OneFileResource.tmp')
  assertTrue(rez:createChild('OneFileResource.tmp', 'One World'))
  assertTrue(lk.exist(fixture.path('project/OneFileResource.tmp')))
  local child = lk.FileResource(childpath, path)
  assertEqual('One World', child:body())

  assertTrue(rez:moveChild(child, rez, 'FileResource.tmp'))
  assertEqual('One World', child:body())
  assertEqual('One World', lk.readall(fixture.path('project/FileResource.tmp')))
  assertFalse(lk.exist(fixture.path('project/OneFileResource.tmp')))
end

function should.mapRoot()
  local path = fixture.path('project')
  local rez = lk.FileResource(path)
  assertNil(rez.ext)
  assertEqual('project', rez.name)
  assertEqual('/', rez.href)
end

test.all()


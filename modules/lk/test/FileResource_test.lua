--[[------------------------------------------------------

  lk.FileResource test
  --------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.FileResource')
local old_date = os.time{year=2011,month=9,day=6}
local new_date = os.time() + 2

local function root()
  return lk.FileResource(fixture.path())
end

function should.teardown()
  lk.rmFile(fixture.path('project/FileResource.tmp'))
  lk.rmFile(fixture.path('project/OneFileResource.tmp'))
  lk.rmTree(fixture.path('project/folder'))
end

function should.autoload()
  assertType('table', lk.FileResource)
end

function should.buildRoot()
  local root = lk.FileResource(fixture.path())
  assertEqual('', root.url)
  assertType('table', root.cache)
end

function should.mapFile()
  local root = root()
  local url = '/project/example.lkp'
  local rez = lk.FileResource(url, root)
  assertEqual('lkp', rez.ext)
  assertEqual('example.lkp', rez.name)
  assertEqual('/project/example.lkp', rez.url)
  assertInRange(old_date, new_date, rez.getlastmodified)
  assertInRange(old_date, new_date, rez.creationdate)
  assertInRange(10, 1000, rez.getcontentlength)
  assertEqual('text/yaml', rez.contenttype)
end

function should.mapDirectory()
  local root = root()
  local proj = lk.FileResource('/project', root)
  assertNil(proj.ext)
  assertEqual('project', proj.name)
  assertEqual('/project', proj.url)
  assertValueEqual({xml = 'collection'}, proj.resourcetype)
  assertInRange(old_date, new_date, proj.getlastmodified)
  assertInRange(old_date, new_date, proj.creationdate)
end

function should.listChildren()
  local root = root()
  local proj = lk.FileResource('/project', root)
  local children = proj:children()
  assertEqual(5, #children)
  assertEqual('bang', children[1].name)
  -- should propagate root
  assertEqual(root, children[2].root)
  assertEqual('example.lkp', children[2].name)
end

function should.cacheInRoot()
  local root = root()
  local proj = lk.FileResource('/project', root)
  local children = proj:children()
  assertEqual(children[1], root.cache['/project/bang'])
  assertEqual(children[2], root.cache['/project/example.lkp'])
end

function should.findFromCache()
  local root = root()
  local proj = lk.FileResource('/project', root)
  local children = proj:children()
  assertEqual(children[1], lk.FileResource('/project/bang', root))
end

function should.delete()
  local root = root()
  local path = fixture.path('project/FileResource.tmp')
  lk.writeall(path, 'one two')
  assertTrue(lk.exist(path))
  local rez = lk.FileResource('/project/FileResource.tmp', root)
  assertTrue(root.cache['/project/FileResource.tmp'])
  assertTrue(rez:delete())
  assertFalse(lk.exist(path))
  -- should be removed from cache
  assertFalse(root.cache['/project/FileResource.tmp'])
end

function should.deleteChildResource()
  local root = root()
  local path = fixture.path()
  local proj = lk.FileResource('/project', root)
  local url = '/project/FileResource.tmp'
  lk.writeall(path .. url, 'one two')
  assertTrue(lk.exist(path .. url))
  -- create children cache
  local children = proj:children()
  assertEqual(6, #children)

  assertTrue(proj:deleteChild(lk.FileResource(url, root)))
  children = proj:children()
  assertEqual(5, #children)
  assertFalse(lk.exist(path .. url))
end

function should.deleteChildName()
  local root = root()
  local proj = lk.FileResource('/project', root)
  local url = '/project/FileResource.tmp'
  local path = fixture.path(url)
  lk.writeall(path, 'one two')
  assertTrue(lk.exist(path))
  -- create children cache
  local children = proj:children()
  assertEqual(6, #children)

  assertTrue(proj:deleteChild('FileResource.tmp'))
  children = proj:children()
  assertEqual(5, #children)
  assertFalse(lk.exist(path))
end

function should.createChild()
  local root = root()
  local proj = lk.FileResource('/project', root)
  -- create children cache
  local children = proj:children()
  assertEqual(5, #children)

  local childpath = fixture.path('project/FileResource.tmp')
  assertTrue(proj:createChild('FileResource.tmp', 'A new FileResource'))
  local child = lk.FileResource('/project/FileResource.tmp', root)
  assertEqual('FileResource.tmp', child.name)
  assertEqual('/project/FileResource.tmp', child.url)
  assertEqual('A new FileResource', lk.readall(childpath))
  assertEqual('A new FileResource', child:body())
  children = proj:children()
  assertEqual(6, #children)
end

function should.createChildDir()
  local root = root()
  local proj = lk.FileResource('/project', root)
  -- create children cache
  local children = proj:children()
  assertEqual(5, #children)

  local childpath = fixture.path('project/folder')
  assertTrue(proj:createChild('folder'))
  local child = lk.FileResource('/project/folder', root)
  assertEqual('folder', child.name)
  assertEqual('/project/folder', child.url)
  assertTrue(child.is_dir)
  children = proj:children()
  assertEqual(6, #children)
  lk.rmTree(childpath)
end

function should.update()
  local root = root()
  local path = fixture.path('project/FileResource.tmp')
  lk.writeall(path, 'A new FileResource')
  local rez = lk.FileResource('/project/FileResource.tmp', root)
  assertEqual('A new FileResource', rez:body())

  assertTrue(rez:update('Something else'))
  assertEqual('Something else', rez:body())
  assertEqual('Something else', lk.readall(path))
end

function should.moveChild()
  local root = root()
  local proj = lk.FileResource('/project', root)
  local childpath = fixture.path('project/OneFileResource.tmp')
  assertTrue(proj:createChild('OneFileResource.tmp', 'One World'))
  assertTrue(lk.exist(fixture.path('project/OneFileResource.tmp')))
  local child = lk.FileResource('/project/OneFileResource.tmp', root)
  assertEqual('One World', child:body())

  assertTrue(proj:moveChild(child, proj, 'FileResource.tmp'))
  assertEqual('One World', child:body())
  assertEqual('One World', lk.readall(fixture.path('project/FileResource.tmp')))
  assertFalse(lk.exist(fixture.path('project/OneFileResource.tmp')))
end

test.all()


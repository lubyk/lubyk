--[[------------------------------------------------------

  lk.FileResource test
  --------------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('lk.FileResource')

function should.teardown()
  lk.rmFile(fixture.path('project/FileResource.tmp'))
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
  local path = fixture.path('project')
  local rez = lk.FileResource(path)
  local children = rez:children()
  assertEqual(5, #children)
  assertEqual('bang', children[1].name)
  -- should propagate rootpath
  assertEqual(fixture.path('project'), children[2].rootpath)
  assertEqual('example.lkp', children[2].name)
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

function should.mapRoot()
  local path = fixture.path('project')
  local rez = lk.FileResource(path)
  assertNil(rez.ext)
  assertEqual('project', rez.name)
  assertEqual('/', rez.href)
end

test.all()


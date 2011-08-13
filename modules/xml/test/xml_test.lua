--[[------------------------------------------------------

  xml test
  --------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('xml')

function should.autoLoad()
  assertTrue(xml)
end

function should.decodeXml()
  local data = xml.parse [[
<document>
  <nodes>
    <a>This is a</a>
    <b name='Bob'>This is Bob</b>
  </nodes>
</document>
]]
  assertTrue(data)
  assertEqual('document', data.xml)
  local b = data:find('b')
  assertEqual('b', b.xml)
  assertEqual('Bob', b.name)
  assertEqual('This is Bob', b[1])
end

function should.removeNamespace()
  local data = xml.parse [[
<?xml version="1.0" encoding="utf-8"?>
<D:propfind xmlns:D="DAV:">
  <D:prop>
    <D:getlastmodified/>
    <D:getcontentlength/>
    <D:creationdate/>
    <D:resourcetype/>
  </D:prop>
</D:propfind>
]]
  assertTrue(data)
  xml.removeNamespace(data, 'DAV:')
  assertEqual('propfind', data.xml)
  local prop = data:find('prop')
  assertEqual('prop', prop.xml)
  assertEqual('getlastmodified', prop[1].xml)
end

function should.encodeXml()
  local data = xml.encode {
    xml = 'Person',
    joe = 'bar',
    'This is bar'
  }
  assertEqual('<Person joe="bar">This is bar</Person>\n', data)
end

test.all()

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

function should.readXml()
  local data = xml.eval [[
<document>
  <nodes>
    <a>This is a</a>
    <b name='Bob'>This is Bob</b>
  </nodes>
</document>
]]
  assertTrue(data)
  local b = data:find('b')
  assertEqual('Bob', b.name)
  assertEqual('This is Bob', b[1])
end

test.all()

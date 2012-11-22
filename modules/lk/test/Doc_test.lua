--[[------------------------------------------------------

  lk.Doc test
  -----------

  ...

--]]------------------------------------------------------
require 'lubyk'
local should = test.Suite('lk.Doc')

function should.autoLoad()
  assertTrue(lk.Doc)
end

function should.extractTitle()
  local doc = lk.Doc(fixture.path('doc/DocTest.lua'))
  assertEqual('test.DocTest', doc.title)
end

function should.extractSummary()
  local doc = lk.Doc(fixture.path('doc/DocTest.lua'))
  assertEqual('This file is a simple test to describe the different documenting options available with lk.Doc. This first paragraph is output as "summary".', doc.summary)
end

function should.extractDescription()
  local doc = lk.Doc(fixture.path('doc/DocTest.lua'))
  assertEqual([[
The following paragraphs up to the end of the preamble comment block define the "description".

A second paragraph in the "description" with an auto link: lk.Doc. And here is a custom link "lubyk":http://lubyk.org.]], doc.description)
end

test.all()



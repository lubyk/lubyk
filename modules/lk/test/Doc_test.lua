--[[------------------------------------------------------

  lk.Doc test
  -----------

  ...

--]]------------------------------------------------------
require 'lubyk'
local should = test.Suite('lk.Doc')

local tmp = fixture.path('tmp')
function should.teardown()
--  lk.rmTree(tmp)
end

function should.autoLoad()
  assertTrue(lk.Doc)
end

function should.extractTitle()
  local doc = lk.Doc(fixture.path('doc/foo/DocTest.lua'))
  assertEqual('DocTest', doc.name)
end

function should.extractSummary()
  local doc = lk.Doc(fixture.path('doc/foo/DocTest.lua'))
  -- first paragraph in first group in first section.
  local summary = doc.sections[1][1][1]
  assertValueEqual({
    text  = 
    'This file is a simple test to describe the different documenting options available with lk.Doc. This first paragraph is output as "summary".',
    class = 'summary',
  }, summary)
end

function should.extractDescription()
  local doc = lk.Doc(fixture.path('doc/foo/DocTest.lua'))
  -- first group of paragraphs
  local description = doc.sections[1][1]
  assertValueEqual({
    { class = 'summary',
      text = 'This file is a simple test to describe the different documenting options available with lk.Doc. This first paragraph is output as "summary".',
    },
    { text = 'The following paragraphs up to the end of the preamble comment block define the "description".'},

    { text = 'A second paragraph in the "description" with an auto link: doc.DocTest. And here is a custom link "lubyk":http://lubyk.org. And some formatting: *strong* _emphasis_.'},

    { text = 'Some lists:'},
    { text = [[* foo
** bar
* baz]]},
    { text = 'Finally, some inline math [math]\\infty[/math] with more text. And now some more math in its own paragraph:'},
    {
      math = 'inline',
      text = '\\frac{\\partial}{\\partial\\theta_j}J(\\theta) = \\frac{1}{m}\\sum_{i=1}^m(\\theta^{T}x^{(i)}-y^{(i)})x_j^{(i)}',
     },
    { text = 'And some more text after math. With an image (the path is relative to the output directory).'},
    { text = '![Dummy example image](img/box.jpg)'},
  }, description)
end

function should.convertToHtml()
  local doc = lk.Doc(fixture.path('doc/foo/DocTest.lua'))
  assertMatch('<title>Documentation Tester</title>', doc:toHtml())
end

function should.makeDoc()
  lk.Doc.make {
    sources = {
      fixture.path('doc'),
    },
    target = tmp,
    format = 'html',
  }
end


test.all()



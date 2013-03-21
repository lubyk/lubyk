--[[------------------------------------------------------

  # Documentation Tester

  This file is a simple test to describe the different documenting options
  available with lk.Doc. This first paragraph is output as "summary".

  The following paragraphs up to the end of the preamble comment block define the
  "description".

  A second paragraph in the "description" with an auto link: doc.DocTest. And here is
  a custom link "lubyk":http://lubyk.org. And some formatting: *strong* _emphasis_.

  Some lists:

  * foo
  ** bar
  * baz

  Finally, some inline math [math]\infty[/math] with more text. And now some more
  math in its own paragraph:

  [math]\frac{\partial}{\partial\theta_j}J(\theta) = \frac{1}{m}\sum_{i=1}^m(\theta^{T}x^{(i)}-y^{(i)})x_j^{(i)}[/math]

  And some more text after math. With an image (the path is relative to the
  output directory).

  ![Dummy example image](img/box.jpg)

--]]------------------------------------------------------

local lib = class 'lk.DocTest'


-- # Create

-- Creating an lk.DocTest is done by providing a table of parameters. 
--
-- Usage example:
--
--   require 'lubyk'
--   local doc = lk.Doc('path/to/File.lua', {target = 'doc'})
--   lk.writeall('doc/File.html', doc:toHtml())
--
-- When documenting multiple files it is better to use #make.
function lib.new(params)
end


-- Generate the documentation for multiple files.
--
-- The `sources` parameter lists file paths to Lua files parse and document. The
-- `target` parameter is the path to the directory where all the output files
-- will be written. `format` is the type of output desired. Possible values are
-- "html" or "latex".
--
-- TODO: an example of a todo inside the comments.
--
-- Usage:
--
--   require 'lubyk'
--   lk.Doc.make {
--     sources = {
--       'lib/doc/DocTest.lua',
--       'lib/doc/Other.lua',
--     },
--     target = 'doc',
--     format = 'html',
--   }
function lib.make(def)
end

local xyz

-- # Parameters
-- This is to test parsing table attributes.
lib.params = { -- doc
  -- This is a first attribute that is used for this or that.
  -- [math]\infty[/math]
  cost1 = {default = 0.5, min = 0, max = 1, unit = 'CHF'},

  -- A second attribute.
  cost2 = {default = 5,   min = 0, max = 10, unit = '$'},

  -- ## Foobar
  -- 
  -- With some text about the Foobar group
  -- of parameters.

  -- An attribute in the foobar group.
  foo = 4,

  -- Another attribute in the foobar group.
  bar = 'some text here',
}
-- comment should be ignored
-- Should not consider 'xyz' as a parameter.
xyz = 4


-- TODO: An orphan todo that should end in the TODO list.

-- # Validation
-- 
-- This is a long description of the new validation section. We can write tons
-- of text here as long as we do not get out of the Lua comments.
--
-- ## Sub-title
--
-- A second paragraph describing the section.
--
-- FIXME: Something to do for validations.


-- Return true if all the documentation went fine. Possible causes of failure
-- are: bad file format, missing assets.
function lib:isOK()

  -- TODO: An orphan todo inside a function that should end in the function def.
end

-- This comment will be ignored.

-- ## String management
-- This group of functions blah, blah.

-- Append the `str` string to the description.
function lib:append(str)
end


-- This definition documents an out of file function. This function takes
-- a `one` and `two` and makes something interesting with these two parameters.
--
-- With a second paragraph to describe the out of file function.
-- function lib:outOfFile(one, two)


--[[
  # Multi line comment

  We can also use a multi-line comment block to define a section with many
  paragraphs.

  Like this a second piece of text with *bold*.

    And 80 columns of source code example
    123456789 123456789 123456789 123456789 123456789 123456789 123456789 1234567890
    123456789 123456789 123456789 123456789 123456789 123456789 123456789 1234567890
--]]


--[[ A long description for a very complex function, blah and this and that. In
  order to help people understand all the details for something very complex.

  Second paragraph before the function.
--]]
function lib:complicated(alpha, beta, gamma)
end



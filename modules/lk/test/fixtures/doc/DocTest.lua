--[[------------------------------------------------------

h1. test.DocTest

This file is a simple test to describe the different documenting options
available with lk.Doc. This first paragraph is output as "summary".

The following paragraphs up to the end of the preamble comment block define the
"description".

A second paragraph in the "description" with an auto link: lk.Doc. And here is
a custom link "lubyk":http://lubyk.org. And some formatting: *strong* _emphasis_.

Some lists:

* foo
** bar
* baz

Finally, some latex [math]\inf[/math].

--]]------------------------------------------------------

local lib = class 'lk.DocTest'


-- h2. Create

-- Creating an lk.DocTest is done by providing a table of parameters. 
--
-- The *sources* paramter lists file paths to Lua files parse and document. The
-- *target* parameter is the path to the directory where all the output files
-- will be written. *format* is the type of output desired. Possible values are
-- "html" or "latex".
function lib.new(params)
end


-- h2. Validation
-- 
-- This is a long description of the new validation section. We can write tons
-- of text here as long as we do not get out of the Lua comments.
--
-- A second paragraph describing the section.


-- Return true if all the documentation went fine. Possible causes of failure
-- are: bad file format, missing assets.
function lib:isOK()
end


-- Append a new paragraph to the description.
function lib:append(str)
end


-- This definition documents an out of file function. This function takes
-- a *one* and *two* and makes something interesting with these two parameters.
--
-- With a second paragraph to describe the out of file function.
-- f. outOfFile(one, two)


--[[
  h2. Multi line comment

  We can also use a multi-line comment block to define a section with many
  paragraphs.

  Like this a second piece of text with *bold*.
--]]


--[[ A long description for a very complex function, blah and this and that. I
  order to help people understand all the details for something very complex.

  Second paragraph before the function.
--]]
function lib:complicated(alpha, beta, gamma)
end



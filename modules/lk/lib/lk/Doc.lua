--[[------------------------------------------------------

  # Documentation extractor

  lk.Doc parses lua comments and extracts the documentation from these comments.

  It parses indifferently multiline comments like this:
  
    --[%%[ 
    Some text in comments.

    Some more text.
    --]%%]

  and single line comments like this:
  
    -- Some text in comments.
    -- 
    -- Some more text.

  # Parsing modes

  By using the special comment @-- doc:[option]@, you can change how the parsing
  is done.

  ## Literate programming

  The parser can generate full script documentation with all lua code when set
  to "lit" (for literate). Turn this option off by setting "nolit".

    -- doc:lit
    -- This part is considered literate programming and all comments and lua
    -- code will be shown in the documentation.
    local x
    
    doSomething(x)

    -- doc:nolit
    -- End of literate part: only the library "lib" will be documented.

  ## Loose documentation

  By setting @-- doc:loose@, the parser will generate a single TODO about
  missing documentation and will not generate any more TODO entries for
  each undocummented function or parameter.
  
  Even if the function names look obvious while writing the code and
  documentation seems superfluous, users will usually appreciate some real
  phrases describing what the function does. Using "loose" is not a good
  idea but can make the documentation more readable if most of the functions
  are not documented.

  # Extraction

  ## Function extraction

  All functions and methods defined against @lib@ are extracted even if they
  are not yet documented. Example:

    function lib:foo(a, b)
      -- Will generate a TODO with "MISSING DOCUMENTATION"
    end

    -- Documented function.
    function lib:bar()
    end

    -- Documented class function.
    function lib.boom()
    end
  
  If a function definition does not follow this convention, it can be
  documented with a commented version of the function definition:

    -- Special function declaration.
    -- function lib:bing(a, b, c)
    lib.bing = lk.Doc.bang

    -- Out of file function definition (in C++ for example).
    -- function lib:connect(server)

  To ignore functions, use @-- nodoc@ as documentation:

    -- nodoc
    function lib:badOldLegacyFunction(x, y)
    end

  ## Table parameters

  All parameters defined against @lib@ are documented unless @-- nodoc@ is used.

    lib.foo = 4
    lib.bar = 5

    -- nodoc
    lib.old_foo = 4

  Since Lua is also used as a description language, it is often useful to
  document table keys. This is done by using @-- doc@. It is a good idea to
  create a new for each table in order to document the table itself.

    -- # Attributes (example)
    -- This is an example of attributes documentation.
    
    local ATTRIBS = { -- doc
      -- Documentation on first key.
      first_name = '',

      -- Documentation on second key.
      last_name = '',

      -- ## Sub-title
      -- Some text for this group of attributes.

      phone = {default = '', format = '000 000 00 00'},
    }

  Such a list will create the following documentation:

--]]

-- # Attributes (example)
-- This is an example of attributes documentation.

local ATTRIBS = { -- doc
  -- Documentation on first key.
  first_name = '',

  -- Documentation on second key.
  last_name = '',

  -- ## Sub-title
  -- Some text for this group of attributes.

  phone = {default = '', format = '000 000 00 00'},
}


--[[

  # Usage

  ## Preamble

  You must start your file with a preample containing a title for the class
  or module and some description.

    --[%%[---------------------------------------
      
      # Full title for the file/class
    
      This first paragraph is the summary.
    
      Some more description.
    
    --]%%]---------------------------------------

  ## Math

  The @[math]@ tag can be used to generate mathematics from latex code. When
  outputing *html*, the page uses [MathJax](http://www.mathjax.org) when outputing html.
  Here is an example of mathematics inline and as standalone paragraph:

    -- Some documentation with inline
    -- [math]\gamma[/math] math. And now a
    -- standalone math paragraph:
    --
    -- [math]\frac{\partial}{\partial\theta_j}J(\theta) = \frac{1}{m}\sum_{i=1}^m(\theta^{T}x^{(i)}-y^{(i)})x_j^{(i)}[/math]

  The result for the code above is:

  Some documentation with inline [math]\gamma[/math] math. And now a
  standalone math paragraph:
  
  [math]\frac{\partial}{\partial\theta_j}J(\theta) = \frac{1}{m}\sum_{i=1}^m(\theta^{T}x^{(i)}-y^{(i)})x_j^{(i)}[/math]

  If you *hover* with the mouse over the formula, it zooms.
  
  ## Lua code
  
  You can insert code snippets by indenting the code with two spaces. Here is
  an example of some normal text and some Lua and C++ code.

    --[%%[
    Some Lua code:

      local foo = {}
      -- print something
      print 'Hello'
    
    And a cpp example:

      #cpp
      float x = 1.0;
      printf("Result = %.2f\n", x);
    --]%%]

  This results in:

  Some Lua code:

    local foo = {}
    -- print something
    print 'Hello'
  
  And a cpp example:

    #cpp
    float x = 1.0;
    printf("Result = %.2f\n", x);

  As you can see, you have to declare other languages with @#name@ if the code
  is not Lua.

  ## Styles

  You can enhance your comments with links, bold, italics and images.

  Some links are automatically created when the parser sees @module.Class@ like
  this: lk.Doc. A custom link is made with @[link title](http://example.com)@.

  Bold is done by using stars: @some text with *bold* emphasis@. Italics are
  inserted with underscores: @some text _with italic_ emphasis@.

  You can add images with @![alt text](/path/to/image.jpg)@.

  ## Lists

  There are two kinds of lists available. The first one is simply a bullet list:

    #txt
    * First element
    * Second element with more text
      that wraps around
    * Third

  Which renders as:

  * First element
  * Second element with more text
    that wraps around
  * Third

  The second style is for definitions:

    #txt
    + some key:       is used to do something.
    + other long key: is used with a very long definition that
                      wraps around and even more text that goes
                      beyond.
    + @last key@:     is a code key.

  Renders as:

  + some key:       is used to do something.
  + other long key: is used with a very long definition that
                    wraps around and even more text that goes
                    beyond.
  + @last key@:     is a code key.

  ## TODO and FIXME

  You can create TODO and FIXME marks like this

    TODO This is a todo definition

    FIXME This is a fixme


  These end up like this (and are repeated at the end of the file).

  TODO This is a todo definition

  FIXME This is a fixme

--]]------------------------------------------------------
local lib     = class 'lk.Doc'
local private = {}
local parser  = {}
local DEFAULT_FOOTER = [[ made with <a href='http://doc.lubyk.org/lk.Doc.html'>lk.Doc</a> ]]


-- # Class functions

-- Parse the content of a file given by @path@ and return an lk.Doc object 
-- containing the documentation of the class.
--
-- Usage example:
--
--   require 'lubyk'
--   local doc = lk.Doc('path/to/File.lua', {target = 'doc'})
--   lk.writeall('doc/File.html', doc:toHtml())
--
-- When documenting multiple files it is better to use #make.
--
-- The @navigation@ and @children@ tables are used to display the 
-- navigation menu on the right and class list in the document body.
function lib.new(path, def)
  def = def or {}
  local self = {
    path   = path,
    target = def.target,
    header = def.header,
    footer = def.footer or DEFAULT_FOOTER,
    navigation = def.navigation or {},
    children   = def.children or {},
    sections = {},
  }
  
  if def.navigation then
    self.module   = self.navigation.__fullname
    self.name     = self.children.__name
    if self.navigation.__fullname then
      self.fullname = self.navigation.__fullname .. '.' .. self.name
    else
      self.fullname = self.name
    end
  else
    self.module, self.name, self.fullname = private.getName(path)
  end

  setmetatable(self, lib)
  if path then
    private.parseFile(self, path)
  else
    -- make dummy doc
    private.newSection(self, 0, self.name)
    table.insert(self.group, {text = '', class = 'summary'})
  end

  if self.children and #self.children > 0 then
    local children = self.children
    local section = self.section
    for _, name in ipairs(children) do
      local child = children[name]

      local group = {
        -- Use 'class' key for children elements.
        class = child.__fullname,
        name  = child.__name,
        child.__summary,
        child.__img,
      }

      if child.__fixme then
        for _, p in ipairs(child.__fixme[1]) do
          table.insert(group, p)
        end
      end

      if child.__todo then
        for _, p in ipairs(child.__todo[1]) do
          table.insert(group, p)
        end
      end

      table.insert(section, group)
    end
  end

  if self.todo then
    table.insert(self.sections, self.todo)
  end

  if self.fixme then
    table.insert(self.sections, self.fixme)
  end

  return self
end


-- Generate the documentation for multiple files.
--
-- The @sources@ parameter lists paths to Lua files or directories to parse and
-- document.
--
-- + target:  parameter is the path to the directory where all the
--            output files will be written.
-- + format:  is the type of output desired. Only 'html' format is supported
--            for now.
-- + sources: lists path to glob for lua files. A source can also be a table
--            with a @prepend@ key used to change the location of the files
--            in the documentation.
-- + copy:    lists the path to glob for static content to copy in @target@.
-- + header:  html code that will be inserted in every html page as header.
-- + footer:  html code that will be inserted in every html page as footer.
--
-- Usage:
--
--   require 'lubyk'
--   lk.Doc.make {
--     sources = {
--       'lib/doc/DocTest.lua',
--       'lib/doc/Other.lua',
--       {'doc', prepend = 'tutorial/foo'},
--     },
--     target = 'doc',
--     format = 'html',
--     header = [[
--       <a href='http://lubyk.org'>
--         <img alt='lubyk logo' src='img/logo.png'/>
--         <h1>Lubyk documentation</h1>
--       </a>
--     ]],
--     footer = [[ made with <a href='lk.Doc.html'>lk.Doc</a> ]],
--   }
function lib.make(def)
  local format = def.format or 'html'
  local output = assert(private.output[format])
  local mod_output = assert(private.mod_output[format])
  -- Prepare output
  lk.makePath(def.target)
  -- Copy base assets
  private.copyAssets[def.format](def.target)
  if def.copy then
    private.copyFiles(def.copy, def.target)
  end


  -- Parse all files and create a tree from the directories and
  -- files to parse.
  -- { name = 'xxxx', sub, { name = 'xxx', subsub }}.
  local tree = {}
  private.parseSources(tree, def.sources)

  private.makeDoc(tree, def)
end

-- # Methods

-- Render the documentation as html. If a @template@ is provided, it is used
-- instead of the default one. This is mainly used for testing since you usually
-- want to have some navigation menus which are extracted by creating the
-- documentation in batch mode with #make.
function lib:toHtml(template)
  return private.output.html(self, template)
end

function private.parseSources(tree, sources)
  local prepend = sources.prepend
  for _, mpath in ipairs(sources) do
    if type(mpath) == 'table' then
      private.parseSources(tree, mpath)
    else
      if lk.fileType(mpath) == 'directory' then
        for path in lk.Dir(mpath):glob '%.lua' do
          private.insertInTree(tree, path, mpath, prepend)
        end
      else
        private.insertInTree(tree, mpath, lk.pathDir(mpath), prepend)
      end
    end
  end
end

function private.insertInTree(tree, fullpath, base, prepend)
  -- Remove base from path
  local path = string.sub(fullpath, string.len(base) + 2, -1)
  if prepend then
    path = prepend .. '/' .. path
  end
  local curr = tree
  local list = lk.split(path, '/')
  local last = #list
  for i, part in ipairs(list) do
    if i == last then
      -- Remove extension
      part = string.match(part, '(.*)%.lua$')
    end

    if not curr[part] then
      local fullname
      if curr.__fullname then
        fullname = curr.__fullname .. '.' .. part
      else
        fullname = part
      end
      curr[part] = { __name = part, __fullname = fullname}
      lk.insertSorted(curr, part)
    end
    curr = curr[part]

    if i == last then
      curr.__file = fullpath
    end
  end
end

function private.makeDoc(tree, def)
  for _, elem_name in ipairs(tree) do
    local elem = tree[elem_name]
    -- Depth first so that we collect all titles and summary first.
    private.makeDoc(elem, def)

    local doc = lib.new(elem.__file, {
      -- Parent & siblings navigation (right menu)
      navigation = tree,
      -- Children navigation (listed in main div)
      children   = elem,
      target     = def.target,
      header     = def.header,
      footer     = def.footer or DEFAULT_FOOTER,
    })
    elem.__title   = doc.sections[1].title
    elem.__summary = doc.sections[1][1][1]
    local img = doc.sections[1][1][2]
    if img and string.match(img.text or '', '^!%[') then
      elem.__img = img
    end
    elem.__todo    = doc.todo
    elem.__fixme   = doc.fixme
    local trg = def.target .. '/' .. doc.fullname .. '.' .. def.format
    lk.writeall(trg, private.output[def.format](doc, def.template))
  end
end


function private.getName(path)
  local name, module, fullname
  name = assert(string.match(path, '([^/]+)%.lua$'), "Invalid path '"..path.."'.")
  module = string.match(path, '([^/]+)/[^/]+$')
  if module then
    fullname = module .. '.' .. name
  else
    fullname = name
  end
  
  return module, name, fullname
end

function private:parseFile(path)
  local file = assert(io.open(path, "r"))
  local line_i = 0
  local state = parser.start
  -- debugging
  for k, v in pairs(parser) do
    v.name = k
  end
  --
  for line in file:lines() do
    local replay = true
    line_i = line_i + 1
    while replay do
      -- if self.name == 'Doc' then
      --   print(string.format("%3i %-14s %s", line_i, state.name or 'SUB', line))
      -- end
      replay = false
      for i=1,#state do
        local matcher = state[i]
        local m = {string.match(line, matcher.match)}
        if m[1] then
          if matcher.output then
            matcher.output(self, line_i, unpack(m))
          end
          local state_exit = state.exit
          if type(matcher.move) == 'function' then
            if state_exit  then state_exit(self) end
            state, replay = matcher.move(self)
            if not state then
              local def = debug.getinfo(matcher.move)
              error("Error in state definition ".. string.match(def.source, '^@(.+)$') .. ':' .. def.linedefined)
            end
            if state.enter then state.enter(self) end
          elseif not matcher.move then
            -- do not change state
          else
            if state_exit  then state_exit(self) end
            state = matcher.move
            if state.enter then state.enter(self) end
          end
          break
        end
      end
    end
  end

  if state.exit then
    state.exit(self)
  end

  if state.eof then
    state.eof(self, line_i)
  end
  -- Clean draft content
  self.para  = nil
  self.scrap = nil
end

--=============================================== Helpers

function private:todoFixme(i, typ, text)
  local group = self.in_func or self.group

  typ = string.lower(typ)
  table.insert(group, self.para)
  local para = {
    span = typ,
    text = text,
  }
  table.insert(group, para)
  local list = self[typ]
  if not list then
    -- Section for todo or fixme
    list = {
      name  = string.upper(typ),
      title = string.upper(typ),
      -- A single group with all fixmes and todos.
      {},
    }
    self[typ] = list
  end
  table.insert(list[1], {
    span  = typ,
    text  = text,
    -- This is to find function reference when the todo is shown
    -- outside the function documentation.
    group = group,
    file  = self.fullname,
    section_name = self.section.name,
  })
  self.para = nil
end

function private:newFunction(i, typ, fun, params)
  local i = #self.group
  if self.group[i] and self.group[i].text == 'nodoc' then
    -- ignore last para
    table.remove(self.group)
    self.para = nil
    return
  end

  -- Store last group as function definition
  if typ == ':' then
    self.group.fun = fun
  elseif typ == '.' then
    self.group.class_fun = fun
  else
    self.group.global_fun = fun
  end
  self.group.params = params
  private.useGroup(self)
  self.in_func = self.group
end

function private:newParam(i, key, params)
  local i = #self.group
  if self.group[i] and self.group[i].text == 'nodoc' then
    -- ignore last para
    table.remove(self.group)
    self.para = nil
    return
  end

  -- The current section becomes the params section.
  self.params = self.section

  -- Store last group as param definition
  self.group.param  = key
  self.group.params = params
  private.useGroup(self)
  self.group = {}
end

function private:newTitle(i, title)
  private.flushPara(self)
  table.insert(self.group, {
    heading = true, text = title
  })
  private.useGroup(self)
end

function private:useGroup()
  local s = self.section
  if s[#s] ~= self.group then
    table.insert(s, self.group)
  end
end

function private:addToPara(i, d)
  if not self.para then
    self.para = { class = self.next_para_class}
    self.next_para_class = nil
  end
  local para = self.para
  if para.text then
    --para.text = para.text .. '\n' .. d or ''
    para.text = para.text .. ' ' .. d or ''
  else
    para.text = d or ''
  end
end

-- Add with newline.
function private:addToParaN(i, d)
  if not self.para then
    self.para = { class = self.next_para_class}
    self.next_para_class = nil
  end
  local para = self.para
  if para.text then
    para.text = para.text .. '\n' .. d or ''
  else
    para.text = d or ''
  end
end

function private:addToList(i, tag, text, definition)
  local key
  if definition then
    key  = text
    text = definition
  end
  local para = self.para
  if not para then
    self.para = {list = {}, text = text, key = key}
  elseif not para.list then
    -- Save previous paragraph.
    private.flushPara(self)
    -- Start new list
    self.para = {list = {}, text = text, key = key}
  else
    -- Move previous element in list
    table.insert(para.list, {text = para.text, key = para.key})
    -- Prepare next.
    para.text = text
    para.key  = key
  end
end

function private:newSection(i, title)
  private.flushPara(self)
  self.group = {}
  self.section = {self.group}
  table.insert(self.sections, self.section)
  self.section.title = title
  local name = title
  if #self.sections == 1 then
    name = self.name
  else
    name = string.gsub(title, ' ', '-')
  end
  self.section.name = name
end

function private:flushPara()
  if self.para then
    table.insert(self.group, self.para)
  end
  self.para = nil
end

--=============================================== Doc parser

-- A parser state is defined with:
-- {MATCH_KEY, SUB-STATES, match = function}
-- list of matches, actions
parser.start = {
  -- matchers
  { match  = '^%-%-%[%[',
    move   = {
      -- h2: new section
      { match  = '^ *# (.+)$',
        output = function(self, i, d)
          private.newSection(self, i, d)
          self.next_para_class = 'summary'
        end,
        move   = function() return parser.mgroup end,
      },
      -- h3: new title
      { match  = '^ *## (.+)$',
        output = private.newTitle,
        move   = function() return parser.mgroup end,
      },
      { match  = '^%-%-%]',
        output = function(self, i)
          print(string.format("Missing '# title' in preamble from '%s'.", self.fullname))
          -- make dummy doc
          private.newSection(self, i, self.fullname)
          table.insert(self.group, {text = '', class = 'summary'})
        end,
        move   = function() return parser.mgroup end,
      },
    }
  },

  eof = function(self, i)
    print(string.format("Reaching end of document without finding preamble documentation in '%s'.", self.fullname))
    -- make dummy doc
    private.newSection(self, i, self.fullname)
    table.insert(self.group, {text = '', class = 'summary'})
  end,
}

-- Multi-line documentation
parser.mgroup = {
  -- End of multi-line comment
  { match  = '^%-%-%]',
    output = private.flushPara,
    move   = function(self) return self.back or parser.end_comment end,
  },
  -- h2: new section
  { match = '^ *# (.+)$',
    output = private.newSection,
  },
  -- h3: new title
  { match = '^ *## (.+)$',
    output = private.newTitle,
  },
  -- out of file function
  { match  = '^ *function lib([:%.])([^%(]+)(.*)$',
    output = private.newFunction,
  },
  -- todo
  { match = '^ *(TODO):? ?(.*)$',
    output = private.todoFixme,
  },
  -- fixme
  { match = '^ *(FIXME):? ?(.*)$',
    output = private.todoFixme,
  },
  -- [math] section
  { match = '^ *%[math%]',
    move  = function() return parser.mmath, true end,
  },
  -- list
  { match = '^ *(%*+) +(.+)$',
    output = private.addToList,
  },
  -- definition list
  { match = '^ *(%+) +(.-): *(.+)$',
    output = private.addToList,
  },
  -- end of paragraph
  { match = '^ *$', 
    output = private.flushPara,
    move = {
      -- code
      { match = '^   ',
        output = private.flushPara,
        move  = function() return parser.mcode, true end,
      },
      { match = '',
        move  = function() return parser.mgroup, true end,
      },
    },
  },
  -- normal paragraph
  { match = '^ *(.+)$',
    output = private.addToPara,
  },
}

parser.mcode = {
  -- first line
  { match  = '^    (.*)$',
    output = function(self, i, d)
      local lang = string.match(d, '#([^ ]+)')
      if lang then
        d = nil
      else
        lang = 'lua'
      end
      lang = string.lower(lang)
      self.para = {code = lang, text = d}
    end,
    move = {
      -- code
      { match  = '^    (.*)$',
        output = private.addToParaN,
      },
      -- empty line
      { match  = '^ *$', 
        output = function(self, i)
          private.addToParaN(self, i, '')
        end,
      },
      -- end of code
      { match  = '', 
        output = function(self, i, d)
          private.flushPara(self)
        end,
        move = function() return parser.mgroup, true end,
      },
    },
  },
}

parser.code = {
  -- first line
  { match  = '^ *%-%-   (.*)$',
    output = function(self, i, d)
      local lang = string.match(d, '#([^ ]+)')
      if lang then
        d = nil
      else
        lang = 'lua'
      end
      lang = string.lower(lang)
      self.para = {code = lang, text = d}
    end,
    move = {
      -- code
      { match  = '^ *%-%-   (.*)$',
        output = private.addToParaN,
      },
      -- empty line
      { match  = '^ *%-%- *$', 
        output = function(self, i)
          private.addToParaN(self, i, '')
        end,
      },
      -- end of code
      { match  = '', 
        output = function(self, i, d)
          private.flushPara(self)
        end,
        move = function() return parser.group, true end,
      },
    },
  },
}

parser.mmath = {
  -- Inline
  { match  = '^ *%[math%](.*)%[/math%]', 
    output = function(self, i, d)
      private.flushPara(self)
      self.para = {math = 'inline', text = d}
      private.flushPara(self)
    end,
    move = function() return parser.mgroup end,
  },
  { match  = '^ *%[math%](.*)',
    output = function(self, i, d)
      private.flushPara(self)
      self.para = {math = 'block', text = d}
    end,
  },
  -- End of math
  { match  = '^(.*)%[/math%]', 
    output = function(self, i, d)
      private.addToPara(self, i, d)
      private.flushPara(self)
    end,
    move = function() return parser.mgroup end,
  },
  { match  = '^(.*)$',
    output = private.addToPara,
  },
}

parser.math = {
  -- One liner
  { match  = '^ *%-%- *%[math%](.*)%[/math%]', 
    output = function(self, i, d)
      private.flushPara(self)
      self.para = {math = true, text = d}
      private.flushPara(self)
    end,
    move = function() return parser.group end,
  },
  { match  = '^ *%-%- *%[math%](.*)',
    output = function(self, i, d)
      private.flushPara(self)
      self.para = {math = true, text = d}
    end,
  },
  -- End of math
  { match  = '^ *%-%- (.*)%[/math%]', 
    output = function(self, i, d)
      private.addToPara(self, i, d)
      private.flushPara(self)
    end,
    move = function() return parser.group end,
  },
  { match  = '^ *%-%- (.*)$',
    output = private.addToPara,
  },
}

parser.group = {
  -- end of comment
  { match  = '^ *[^%- ]',
    output = private.flushPara,
    move   = function(self) return self.back or parser.end_comment, true end
  },
  { match  = '^ *$',
    output = private.flushPara,
    move   = function(self)
      if self.back then
        return self.back, true
      else
        return parser.lua
      end
    end,
  },
  -- h2: new section
  { match  = '^ *%-%- *# (.+)$',
    output = private.newSection,
  },
  -- h3: new title
  { match  = '^ *%-%- *## (.+)$',
    output = private.newTitle,
  },
  -- out of file function definition
  { match  = '^ *%-%- *function lib([:%.])([^%(]+)(.*)$',
    output = private.newFunction,
  },
  -- [math] section
  { match = '^ *%-%- *%[math%]',
    move  = function() return parser.math, true end,
  },
  -- todo
  { match = '^ *%-%- *(TODO):? ?(.*)$',
    output = private.todoFixme,
  },
  -- fixme
  { match = '^ *%-%- *(FIXME):? ?(.*)$',
    output = private.todoFixme,
  },
  -- list
  { match = '^ *%-%- *(%*+) +(.+)$',
    output = private.addToList,
  },
  -- definition list
  { match = '^ *%-%- *(%+) +(.-): *(.+)$',
    output = private.addToList,
  },
  -- end of paragraph
  { match = '^ *%-%- *$', 
    output = private.flushPara,
    move = {
      -- code
      { match = '^ *%-%-   ',
        output = private.flushPara,
        move  = function() return parser.code, true end,
      },
      { match = '',
        move  = function() return parser.group, true end,
      },
    },
  },
  -- normal paragraph
  { match = '^ *%-%- *(.+)$',
    output = private.addToPara,
  },
}

-- This is called just after the comment block ends.
parser.end_comment = {
  -- lib function
  { match  = '^function lib([:%.])([^%(]+) *(%(.-%))',
    output = private.newFunction,
    move   = function() return parser.lua end,
  },
  -- lib param
  { match  = 'lib%.([a-zA-Z_0-9]+) *= *(.+)$',
    output = function(self, i, key, def)
      if self.group[1] and self.group[1].heading then
        -- Group is not for us
        self.group = {}
        if not self.loose then
          private.todoFixme(self, i, 'TODO', 'MISSING DOCUMENTATION')
        end
        private.newParam(self, i, key, def)
      else
        private.newParam(self, i, key, def)
      end
    end
  },
  -- global function
  { match  = '^function ([^:%.%(]+) *(%(.-%))',
    output = function(self, i, name, params)
      private.newFunction(self, i, '', name, params)
    end
  },
  -- Match anything moves to raw code
  { match = '',
    move = function(self) return parser.lua, true end,
  }
}

parser.lua = {
  enter = function(self)
    if self.lit then
      -- Make sure we use previous comment.
      private.useGroup(self)
      self.group = {}
      self.para = {code = 'lua'}
    end
  end,
  exit = function(self)
    if self.lit then
      private.flushPara(self)
      private.useGroup(self)
    end
  end,
  -- Undocummented function
  { match  = '^(function lib([:%.])([^%(]+) *(%(.-%)).*)$',
    output = function(self, i, all, typ, fun, params)
      if self.lit then
        private.addToParaN(self, i, all)
      else
        self.group = {}
        if not self.loose then
          private.todoFixme(self, i, 'TODO', 'MISSING DOCUMENTATION')
        end
        private.newFunction(self, i, typ, fun, params)
      end
    end,
  },
  -- Undocummented param
  { match  = '^(lib%.([a-zA-Z_0-9]+) *= *(.+))$',
    output = function(self, i, all, param, def)
      if self.lit then
        private.addToParaN(self, i, all)
      else
        if not self.loose then
          private.todoFixme(self, i, 'TODO', 'MISSING DOCUMENTATION')
        end
        private.newParam(self, i, param, def)
      end
    end,
  },
  -- end of function
  { match  = '^(end.*)$',
    output = function(self, i, d)
      self.in_func = nil
      if self.lit then private.addToParaN(self, i, d) end
    end,
  },
  -- move out of literate programming
  { match  = '^%-%- doc:no(.+)$',
    output = function(self, i, d)
      self[d] = false
    end,
  },
  -- enter literate programming
  { match  = '^%-%- doc:(.+)$',
    output = function(self, i, d)
      if d == 'loose' then
        private.todoFixme(self, i, 'TODO', 'INCOMPLETE DOCUMENTATION')
      end
      self[d] = true
    end,
  },
  -- params
  { match  = '{ %-%- *doc *$',
    move   = function() return parser.params end,
  },
  -- todo
  { match = '^ *%-%- *(TODO):? ?(.*)$',
    output = function(self, ...)
      private.todoFixme(self, ...)
    end,
  },
  -- fixme
  { match = '^ *%-%- *(FIXME):? ?(.*)$',
    output = function(self, ...)
      private.todoFixme(self, ...)
    end,
  },
  { match  = '^ *%-%- +(.+)$',
    move = function(self)
      -- Temporary group (not inserted in section).
      self.group = {}
                           -- replay last line
      return parser.group, true
    end,
  },
  { match  = '^%-%-%[%[',
    output = function(self)
      -- Temporary group (not inserted in section).
      self.group = {}
    end,
    move = function() return parser.mgroup end,
  },
  { match = '^(.*)$',
    output = function(self, ...)
      if self.lit then
        private.addToParaN(self, ...)
      end
    end,
  },
}

parser.params = {
  { match  = '^ *%-%- +(.+)$',
    output = function(self, i, d)
      -- Temporary group (not inserted in section).
      self.group = {}
    end,
    move = function(self)
      self.back = parser.params
      -- replay last line
      return parser.group, true
    end,
  },
  { match  = '^%-%-%[%[ *(.*)$',
    output = function(self, i, d)
      private.addToPara(self, i, d)
      -- Temporary group (not inserted in section).
      self.group = {}
    end,
    move = function(self)
      self.back = parser.params
      return parser.mgroup
    end,
  },
  -- param definition
  { match  = '^ *([a-zA-Z0-9_]+) *= *(.*), *$',
    output = private.newParam,
  },
  -- end of params definition
  { match = '^}',
    move = function(self)
      self.back = nil
      return parser.lua
    end
  },
  { match = '',
    output = function(self)
      private.flushPara(self)
      self.group = {}
    end,
  },
}

--=============================================== 

-- Output individual class definitions
private.output = {}
-- Output module with class summary
private.mod_output = {}

private.copyAssets = {}

function private.getTemplate(format)
  local filename = 'template.'..format
  return lk.content(lk.scriptDir()..'/doc/'..filename)
end

--=============================================== HTML TEMPLATE
function private.output:html(template)
  local tmplt = dub.Template(template or private.getTemplate('html'))
  return tmplt:run {self = self, private = private}
end

function private.mod_output.html(module, def, modules)
  local tmplt = dub.Template(def.template or private.getTemplate('html'))
  -- Create a pseudo class with classes as methods and class summary
  -- as method documentation.
  local self = {
    name = module.name,
    title = module.name,
    fullname = module.name,
    sections = {},
    navigation = modules,
    header = def.header,
    footer = def.footer or DEFAULT_FOOTER,
  }
  local section = {name = modules.name, title = module.name}
  table.insert(self.sections, section)
  for _, class in ipairs(module) do
    local def = module[class]
    -- A group = class doc
    table.insert(section, {
      class = def.fullname,
      name  = def.name,
      { text = def.summary },
    })
  end
  setmetatable(self, lib)
  return tmplt:run {self = self, private = private}
end

function private.copyFiles(list, target)
  for _, mpath in ipairs(list) do
    local len = string.len(mpath)
    for src in lk.Dir(mpath):glob() do
      local path = string.sub(src, len + 2)
      local trg  = target .. '/' .. path
      lk.copy(src, trg)
    end
  end
end

function private.copyAssets.html(target)
  local src_base = lk.scriptDir()
  for _, path in ipairs {
    'css/bootstrap.css',
    'css/docs.css',
    'img/glyphicons-halflings-white.png', 
    'img/glyphicons-halflings.png',
    'js/bootstrap.min.js',
  } do
    local src = src_base .. '/doc/' .. path
    local trg = target .. '/' .. path
    lk.writeall(trg, lk.content(src))
  end
end

function private:paraToHtml(para)
  local text = para.text or ''
  if para.class then
    return "<p class='"..para.class.."'>"..private.textToHtml(self, text).."</p>"
  elseif para.heading then
    return "<h4 class='sub-title'>"..private.textToHtml(self, text).."</h4>"
  elseif para.math then
    return "<p>"..private.mathjaxTag(self, para).."</p>"
  elseif para.code then
    local tag
    if para.code == 'txt' then
      tag = "<pre>"
    else
      tag = "<pre class='prettyprint lang-"..para.code.."'>"
    end
    return tag .. 
      string.gsub(
      string.gsub(
      string.gsub(text,
        '<', '&lt;'
      ),
        '>', '&gt;'
      ),
        '%%%%', ''
      )..
      "</pre>"
  elseif para.span then
    return private.spanToHtml(self, para)
  elseif para.list then
    -- render list
    return private.listToHtml(self, para)
  else
    return "<p>"..private.textToHtml(self, text).."</p>"
  end
end

function private:spanToHtml(para)
  local ref = ''
  local ref_name
  if para.group then
    if self.fullname ~= para.file then
      ref = para.file .. '.html'
    end
    if para.group.fun then
      if ref then
        ref = ref .. '#' .. para.group.fun
      else
        ref = '#' .. para.group.fun
      end
      ref_name = '#' .. para.group.fun
    elseif para.section_name then
      if ref then
        ref = ref .. '#' .. para.section_name
      else
        ref = '#' .. para.section_name
      end
      ref_name = para.section_name
    end
    ref = "<span class='ref'><a href='"..ref.."'>"..ref_name.."</a></span>"
  end
  return "<p class='"..para.span.."'>" ..
         ref ..
         "<span>"..string.upper(para.span).."</span> "..
         private.textToHtml(self, para.text)..
         "</p>"
end

function private:textToHtml(text)
  -- filter content
  local p = text or ''
  -- TODO: Replace textToHtml with a walking parser to avoid double parsing.
  -- code
  local codes = {}
  p = string.gsub(p, '@(.-)@', function(code)
    table.insert(codes, '<code>'..code..'</code>')
    return 'CODE'..#codes
  end)
  -- strong
  p = string.gsub(p, '%*([^\n]-)%*', '<strong>%1</strong>')
  -- em
  p = string.gsub(p, '_(.-)_', '<em>%1</em>')
  -- method link lk.Doc#make
  p = string.gsub(p, ' ([a-z]+%.[A-Z]+[a-z][a-zA-Z]+)#([a-zA-Z_]+)', " <a href='%1.html#%2'>%1#%2</a>")
  -- auto-link lk.Doc
  p = string.gsub(p, ' ([a-z]+%.[A-Z]+[a-z][a-zA-Z]+)([%. ])', " <a href='%1.html'>%1</a>%2")
  -- section link #Make or method link #foo
  p = string.gsub(p, ' #([A-Za-z]+[A-Za-z_]+)', " <a href='#%1'>%1</a>")
  -- ![Dummy example image](img/box.jpg)
  p = string.gsub(p, '!%[(.-)%]%((.-)%)', "<img alt='%1' src='%2'/>")
  -- link [some text](http://example.com)
  p = string.gsub(p, '%[([^%]]+)%]%(([^%)]+)%)', function(text, href)
    return "<a href='"..href.."'>"..text.."</a>"
  end)
  p = string.gsub(p, '%[math%](.-)%[/math%]', function(latex)
    return private.mathjaxTag(self, {math = 'inline', text = latex})
  end)

  if #codes > 0 then
    p = string.gsub(p, 'CODE([0-9]+)', function(id)
      return codes[tonumber(id)]
    end)
  end
  return p
end

function private:listToHtml(para)
  if para.text then
    -- Flush last list element.
    table.insert(para.list, {text = para.text, key = para.key})
    para.text = nil
    para.key  = nil
  end

  if para.list[1].key then
    -- definition list
    local out = "<table class='definition'>\n"
    for _, line in ipairs(para.list) do
      out = out .. "  <tr><td class='key'>"..
            private.textToHtml(self, line.key) .."</td><td>" ..
            private.textToHtml(self, line.text).."</td></tr>\n"
    end
    return out .. '\n</table>'
  else
    -- bullet list
    local out = '<ul>\n'
    for _, line in ipairs(para.list) do
      out = out .. '<li>' .. private.textToHtml(self, line.text) .. '</li>\n'
    end
    return out .. '</ul>'
  end
end

local function osTry(cmd)
  local ret = os.execute(cmd)
  if ret ~= 0 then
    printf("Could not execute '%s'.", cmd)
  end
  return ret
end

function private:mathjaxTag(para)
  if para.math == 'inline' then
    return '\\('..para.text..'\\)'
  else
    return '$$'..para.text..'$$'
  end
end

function private:latexImageTag(para)
  local target = self.target
  local latex = para.text
  local mock = '[latex]'..latex..'[/latex]'
  -- Cannot process latex if we do not have an output target
  if not target then return mock end

  local pre, post = '', ''
  local type = string.match(latex, '^ *\\begin\\{(.-)}')
  if not type or
    (type ~= 'align' and
    type ~= 'equation' and
    type ~= 'itemize') then
    pre = '\\['
    post = '\\]'
  end

  if self.latex_img_i then
    self.latex_img_i = self.latex_img_i + 1
  else
    self.latex_img_i = 1
  end
  local img_name = self.fullname .. '.' .. self.latex_img_i .. '.png'
  local img_id   = 'latex'..self.latex_img_i

  local template = dub.Template(private.LATEX_IMG_TEMPLATE)
  local content = template:run { pre = pre, latex = latex, post = post }
  -- Create tmp file
  -- write content
  -- create image
  -- copy image to target/latex/doc.DocTest.1.png
  -- return image tag
  local tempf = 'tmpf' .. math.random(10000000, 99999999)

  lk.makePath(tempf)
  lk.makePath(target .. '/latex')
  lk.writeall(tempf .. '/base.tex', content)
  if osTry(string.format('cd %s && latex -interaction=batchmode "base.tex" &> /dev/null', tempf)) ~= 0 then
    lk.rmTree(tempf, true)
    return mock
  end
  if osTry(string.format('cd %s && dvips base.dvi -E -o base.ps &> /dev/null', tempf)) ~= 0 then
    lk.rmTree(tempf, true)
    return mock
  end
  if osTry(string.format('cd %s && convert -density 150 base.ps -matte -fuzz 10%% -transparent "#ffffff" base.png', tempf, target, img_name)) ~= 0 then
    lk.rmTree(tempf, true)
    return mock
  end
  if osTry(string.format('mv %s/base.png %s/latex/%s', tempf, target, img_name)) ~= 0 then
    lk.rmTree(tempf, true)
    return mock
  end
  lk.rmTree(tempf, true)
  return string.format("<code id='c%s' class='prettyprint lang-tex' style='display:none'>%s</code><img class='latex' id='%s' onclick='$(\"#c%s\").toggle()' src='latex/%s'/>", img_id, latex, img_id, img_id, img_name)
end

private.LATEX_IMG_TEMPLATE = [=[
\documentclass[10pt]{article}
\usepackage[utf8]{inputenc}
\usepackage{amssymb}

\usepackage{amsmath}
\usepackage{amsfonts}
% \usepackage{ulem}     % strikethrough (\sout{...})
\usepackage{hyperref} % links


% shortcuts
\DeclareMathOperator*{\argmin}{arg\,min}
\newcommand{\ve}[1]{\boldsymbol{#1}}
\newcommand{\ma}[1]{\boldsymbol{#1}}
\newenvironment{m}{\begin{bmatrix}}{\end{bmatrix}}

\pagestyle{empty}
\begin{document}
{{pre}}
{{latex}}
{{post}}
\end{document}
}
]=]

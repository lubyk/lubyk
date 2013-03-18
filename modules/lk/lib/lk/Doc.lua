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

  The "math" tag can be used to generate mathematics from latex code. When
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
function lib.new(path, def)
  def = def or {}
  local self = {
    path   = path,
    target = def.target,
    header = def.header,
    footer = def.footer or DEFAULT_FOOTER,
    navigation = def.navigation or {},
    sections = {
   --   {name='create'}, {name='compute'}
    },
    -- This is the 'section' that describes the class itself.
    info = {},
  }
  self.module, self.name, self.fullname = private.getName(path)

  setmetatable(self, lib)
  private.parseFile(self, path)
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
-- document. The @target@ parameter is the path to the directory where all the
-- output files will be written. @format@ is the type of output desired.
-- Only 'html' format is supported for now.
--
-- You can also provide some html code for @header@ and @footer@ that will be
-- inserted in every html output page.
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
  -- Copy assets
  private.copyAssets[def.format](def.target)

  -- Parse all files
  local modules = {}
  for _, mpath in ipairs(def.sources) do
    local path
    if lk.fileType(mpath) == 'directory' then
      for path in lk.Dir(mpath):glob '%.lua' do
        private.addPathToModules(modules, path, mpath)
      end
    else
      private.addPathToModules(modules, mpath, mpath)
    end
  end

  for _, module_name in ipairs(modules) do
    local module = modules[module_name]
    local base_def = {
      navigation = module,
      target  = def.target,
      modules = modules,
      header  = def.header,
      footer  = def.footer or DEFAULT_FOOTER,
    }
    for _, fullname in ipairs(module) do
      local path = module[fullname]
      local doc = lib.new(path, base_def)
      module[fullname] = {
        fullname = doc.fullname,
        name     = doc.name,
        title    = doc.sections[1].title,
        summary  = doc.sections[1][1][1].text,
      }
      local trg = def.target .. '/' .. doc.fullname .. '.' .. format
      lk.writeall(trg, output(doc, def.template))
    end
    -- Create module page
    local trg = def.target .. '/' .. module.name .. '.' .. format
    lk.writeall(trg, mod_output(module, def, modules))
  end
end

-- # Methods

-- Render the documentation as html. If a @template@ is provided, it is used
-- instead of the default one. This is mainly used for testing since you usually
-- want to have some navigation menus which are extracted by creating the
-- documentation in batch mode with #make.
function lib:toHtml(template)
  return private.output.html(self, template)
end

function private.addPathToModules(modules, path, base)
  local module, name, fullname = private.getName(path, base)
  if not name then
    -- This is the module's lua file. We should parse it but for
    -- now, we simply ignore it.
    return
  end
  local list = modules[module]
  if not list then
    list = {name = module}
    modules[module] = list
    lk.insertSorted(modules, module)
  end
  lk.insertSorted(list, fullname)
  list[fullname] = path
end

function private.getName(path, base)
  local name, module, fullname
  if not base then
    name = assert(string.match(path, '([^/]+)%.lua$'), "Invalid path '"..path.."'.")
    module = string.match(path, '([^/]+)/[^/]+$')
    if module then
      fullname = module .. '.' .. name
    else
      fullname = name
    end
  else
    path = string.sub(path, string.len(base) + 2, -1)
    module = assert(string.match(path, '^([^/]+)'), path)
    if module == path then
      -- This is the module's lua file. We should parse it but for
      -- now, we simply ignore it.
      module = string.sub(path, 1, -5)
      name   = nil
      fullname = module
    else
      name = string.gsub(string.sub(path, string.len(module) + 2, -5), '/', '.')
      fullname = module .. '.' .. name
    end
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
      --print(string.format("%3i %-14s %s", line_i, state.name or 'SUB', line))
      replay = false
      for i=1,#state do
        local matcher = state[i]
        local m = {string.match(line, matcher.match)}
        if m[1] then
          if matcher.output then
            matcher.output(self, line_i, unpack(m))
          end
          if type(matcher.move) == 'function' then
            state, replay = matcher.move(self)
            if not state then
              local def = debug.getinfo(matcher.move)
              error("Error in state definition ".. string.match(def.source, '^@(.+)$') .. ':' .. def.linedefined)
            end
          elseif matcher.move == false then
            -- do not change state
          else
            state = matcher.move or state
          end
          break
        end
      end
    end
  end

  if state.eof then
    state.eof(self, line_i)
  end
  -- Clean draft content
  self.para  = nil
  self.scrap = nil
end

--=============================================== Helpers

function private:todoFixme(i, typ, text, text2)
  typ = string.lower(typ)
  table.insert(self.group, self.para)
  local para = {
    span = typ,
    text = text,
  }
  table.insert(self.group, para)
  local list = self[typ]
  if not list then
    -- Section for TODO or FIXME
    list = {
      name  = string.upper(typ),
      title = string.upper(typ),
      -- A single group with all fixmes and todos.
      {},
    }
    self[typ] = list
  end
  if text2 then
    table.insert(list[1], {
      span = typ,
      text = text2,
    })
  else
    table.insert(list[1], para)
  end
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
  if self.section[#self.section] ~= self.group then
    -- This can happen if we are using a temporary group.
    table.insert(self.section, self.group)
  end
end

function private:newTitle(i, title)
  private.flushPara(self)
  table.insert(self.group, {
    heading = true, text = title
  })
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
    move   = function() return parser.end_comment end,
  },
  -- h2: new section
  { match = '^ *# (.+)$',
    output = private.newSection,
  },
  -- h3: new title
  { match = '^ *## (.+)$',
    output = private.newTitle,
  },
  -- code
  { match = '^   ',
    output = private.flushPara,
    move  = function() return parser.mcode, true end,
  },
  -- out of file function
  { match  = '^ *function lib([:%.])([^%(]+)(.*)$',
    output = private.newFunction,
  },
  -- TODO
  { match = '^ *(TODO):? (.*)$',
    output = private.todoFixme,
  },
  -- FIXME
  { match = '^ *(FIXME):? (.*)$',
    output = private.todoFixme,
  },
  -- [math] section
  { match = '^ *%[math%]',
    move  = function() return parser.mmath, true end,
  },
  -- List
  { match = '^ *(%*.+)$',
    output = private.addToParaN,
  },
  -- Normal paragraph
  { match = '^ *(.+)$',
    output = private.addToPara,
  },
  -- End of paragraph
  { match = '^ *$', 
    output = private.flushPara,
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
  { match  = '^%-%-   (.*)$',
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
      { match  = '^%-%-   (.*)$',
        output = private.addToParaN,
      },
      -- empty line
      { match  = '^%-%- *$', 
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
  { match  = '^%-%- *%[math%](.*)%[/math%]', 
    output = function(self, i, d)
      private.flushPara(self)
      self.para = {math = true, text = d}
      private.flushPara(self)
    end,
    move = function() return parser.group end,
  },
  { match  = '^%-%- *%[math%](.*)',
    output = function(self, i, d)
      private.flushPara(self)
      self.para = {math = true, text = d}
    end,
  },
  -- End of math
  { match  = '^%-%- (.*)%[/math%]', 
    output = function(self, i, d)
      private.addToPara(self, i, d)
      private.flushPara(self)
    end,
    move = function() return parser.group end,
  },
  { match  = '^%-%- (.*)$',
    output = private.addToPara,
  },
}

parser.group = {
  -- End of comment
  { match    = '^[^%-]',
    output = private.flushPara,
    move = function() return parser.end_comment, true end
  },
  { match    = '^$',
    output = private.flushPara,
    move = function() return parser.lua end
  },
  -- h2: new section
  { match = '^%-%- *# (.+)$',
    output = private.newSection,
  },
  -- h3: new title
  { match = '^%-%- *## (.+)$',
    output = private.newTitle,
  },
  -- code
  { match = '^%-%-   ',
    output = private.flushPara,
    move  = function() return parser.code, true end,
  },
  -- out of file function definition
  { match  = '^%-%- *function lib([:%.])([^%(]+)(.*)$',
    output = private.newFunction,
  },
  -- [math] section
  { match = '^%-%- *%[math%]',
    move  = function() return parser.math, true end,
  },
  -- TODO
  { match = '^%-%- *(TODO):? (.*)$',
    output = private.todoFixme,
  },
  -- FIXME
  { match = '^%-%- *(FIXME):? (.*)$',
    output = private.todoFixme,
  },
  -- List
  { match = '^%-%- *(%*.+)$',
    output = private.addToParaN,
  },
  -- Normal paragraph
  { match = '^%-%- *(.+)$',
    output = private.addToPara,
  },
  -- End of paragraph
  { match = '^%-%- *$', 
    output = private.flushPara,
  },
}

-- This is called just after the comment block ends.
parser.end_comment = {
  -- lib function
  { match  = '^function lib([:%.])([^%(]+) *(%(.-%))',
    output = private.newFunction,
  },
  -- global function
  { match  = '^function ([^%(]+) *(%(.-%))',
    output = function(self, i, name, params)
      private.newFunction(self, i, '', name, params)
    end
  },
  -- Match anything moves to raw code
  { match = '',
    move = function() return parser.lua end,
  }
}

parser.lua = {
  { match  = '^function lib([:%.])([^%(]+) *(%(.-%))',
    output = function(self, i, typ, fun, params)
      self.group = {}
      local text = string.format("MISSING DOCUMENTATION FOR #%s.", fun)
      private.todoFixme(self, i, 'TODO', 'MISSING DOCUMENTATION', text)
      private.newFunction(self, i, typ, fun, params)
    end,
  },
  { match  = '^%-%- +(.+)$',
    output = function(self, i, d)
      -- Temporary group (not inserted in section).
      self.group = {}
    end,                                   -- replay last line
    move = function() return parser.group, true end,
  },
  { match  = '^%-%-%[%[ *(.*)$',
    output = function(self, i, d)
      private.addToPara(self, i, d)
      -- Temporary group (not inserted in section).
      self.group = {}
    end,
    move = function() return parser.mgroup end,
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

function private:paraToHtml(para, inline)
  local text = para.text or ''
  if para.class then
    return "<p class='"..para.class.."'>"..private.textToHtml(self, text).."</p>"
  elseif para.heading then
    return "<h4 class='sub-title'>"..private.textToHtml(self, text).."</h4>"
  elseif para.math then
    return "<p>"..private.mathjaxTag(self, para).."</p>"
  elseif para.code then
    return "<p><pre class='prettyprint lang-"..para.code.."'>"..
      string.gsub(
      string.gsub(
      string.gsub(text,
        '<', '&lt;'
      ),
        '>', '&gt;'
      ),
        '%%%%', ''
      )..
      "</pre></p>"
  elseif para.span then
    return "<p><span class='"..para.span.."'><span>"..string.upper(para.span).."</span> "..private.textToHtml(self, text).."</span></p>"
  elseif not inline and string.match(text, '^%*') then
    -- render list
    return private.listToHtml(self, para)
  else
    return "<p>"..private.textToHtml(self, text).."</p>"
  end
end

function private:textToHtml(text)
  -- filter content
  local p = text or ''
  -- FIXME: replace with walking parser to avoid double parsing.
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
  -- method link lk.Doc.make
  p = string.gsub(p, ' ([a-z]+%.[A-Z]+[a-z][a-zA-Z]+)%.([a-zA-Z_]+)', " <a href='%1.html#%2'>%1.%2</a>")
  -- auto-link lk.Doc
  p = string.gsub(p, ' ([a-z]+%.[A-Z]+[a-z][a-zA-Z]+)([%. ])', " <a href='%1.html'>%1</a>%2")
  -- method link #make
  p = string.gsub(p, ' #([a-z]+[A-Za-z_]+)', " <a href='#%1'>%1</a>")
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
  local out = '<ul>\n'
  for _, line in ipairs(lk.split(para.text, '\n')) do
    local depth, text = string.match(line, '^(%*+) (.*)$')
    out = out .. '<li>' .. private.paraToHtml(self, text, true) .. '</li>\n'
  end
  return out .. '</ul>'
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

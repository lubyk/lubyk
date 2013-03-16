--[[------------------------------------------------------

  lk.Doc
  ------

  Documentation generator for lubyk. This generator parses the text using a
  state machine made of current triggers and transitions.

  The "latex" tag can be used to generate images from latex code when outputing
  html. For this to work, you need to have @latex@, @dvips@ and @convert@ 
  installed. On mac a minimal latex distribution can be found at "tug.org":http://www.tug.org/mactex/morepackages.html.

--]]------------------------------------------------------
local lib     = class 'lk.Doc'
local private = {}
local parser  = {}

-- Parse the content of a file given by *path* and return a lk.Doc object 
-- containing the documentation of the class.
function lib.new(path, def)
  def = def or {}
  local self = {
    path   = path,
    target = def.target,
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

  return self
end

function lib:toHtml(template)
  return private.output.html(self, template)
end

function lib.make(def)
  local format = def.format or 'html'
  local output = assert(private.output[format])
  -- Prepare output
  lk.makePath(def.target)
  -- Copy assets
  private.copy_assets[def.format](def.target)

  -- Parse all files
  local list = {}
  for _, path in ipairs(def.sources) do
    local _, _, fullname = private.getName(path)
    lk.insertSorted(list, fullname)
  end

  for _, path in ipairs(def.sources) do
    local doc = lib.new(path, {target = def.target, navigation = list})
    local trg = def.target .. '/' .. doc.fullname .. '.' .. format
    lk.writeall(trg, output(doc, def.template))
  end
end

function private.getName(path)
  local name   = assert(string.match(path, '([^/]+)%.lua$'))
  local module = string.match(path, '([^/]+)/[^/]+$')
  local fullname
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

function private:newSection(title)
  private.flushPara(self)
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
    if #self.group > 0 and self.group.class == 'summary' then
      -- Create new group before inserting
      self.group = {}
      table.insert(self.section, self.group)
    end
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
      -- h1 / h2: new section
      { match  = '^ *(h[12])%. (.+)$',
        output = function(self, i, tag, d)
          private.newSection(self, d)
          self.group = {class = 'summary'}
          table.insert(self.section, self.group)
        end,
        move   = function() return parser.mgroup end,
      },
      { match  = '^%-%-%]',
        output = function(self, i) error('Missing "h1. title" in preamble.') end
      },
    }
  },

  eof = function(self, i) error('Reaching end of document without finding preamble documentation.') end,
}

-- Multi-line documentation
parser.mgroup = {
  -- End of multi-line comment
  { match  = '^%-%-%]',
    output = private.flushPara,
    move   = function() return parser.end_comment end,
  },
  -- h1 / h2: new section
  { match = '^ *(h[12])%. (.+)$',
    output = function(self, i, tag, d)
      private.newSection(self, d)
    end,
  },
  -- [math] section
  { match = '^ *%[math%]',
    move  = function() return parser.mmath, true end,
  },
  -- List
  { match = '^ *(%*.+)$',
    output = function(self, i, d)
      if self.para then
        self.para = self.para .. '\n' .. d
      else
        self.para = d
      end
    end,
  },
  -- Normal paragraph
  { match = '^ *(.+)$',
    output = function(self, i, d)
      if self.para then
        self.para = self.para .. ' ' .. d
      else
        self.para = d
      end
    end,
  },
  -- End of paragraph
  { match = '^ *$', 
    output = private.flushPara,
  },
}

parser.mmath = {
  -- One liner
  { match  = '^ *%[math%](.*)%[/math%]', 
    output = function(self, i, d)
      self.group = {math = true}
      table.insert(self.section, self.group)
      self.para = d
      private.flushPara(self)
    end,
    move = function() return parser.mgroup end,
  },
  { match  = '^ *%[math%](.*)',
    output = function(self, i, d)
      self.group = {math = true}
      table.insert(self.section, self.group)
      self.para = d
    end,
  },
  -- End of math
  { match  = '^(.*)%[/math%]', 
    output = function(self, i, d)
      self.para = self.para .. '\n' .. d
      private.flushPara(self)
    end,
    move = function() return parser.mgroup end,
  },
  { match  = '^(.*)$',
    output = function(self, i, d)
      self.para = self.para .. '\n' .. d
    end,
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
    move = function() return parser.code end
  },
  -- h1 / h2: new section
  { match = '^%-%- *(h[12])%. (.+)$',
    output = function(self, i, tag, d)
      private.newSection(self, d)
    end,
  },
  -- List
  { match = '^%-%- *(%*.+)$',
    output = function(self, i, d)
      if self.para then
        self.para = self.para .. '\n' .. d
      else
        self.para = d
      end
    end,
  },
  -- Normal paragraph
  { match = '^%-%- *(.+)$',
    output = function(self, i, d)
      if self.para then
        self.para = self.para .. ' ' .. d
      else
        self.para = d
      end
    end,
  },
  -- End of paragraph
  { match = '^%-%- *$', 
    output = function(self, i, d)
      if #self.group > 0 and self.group.class == 'summary' then
        -- Create new group before inserting
        self.group = {}
        table.insert(self.section, self.group)
      end
      table.insert(self.group, self.para)
      self.para = nil
    end,
  },
}

-- This is called just after the comment block ends.
parser.end_comment = {
  { match  = '^function lib([:%.])([^%(]+)(.*)$',
    output = function(self, i, typ, fun, params)
      -- Store last group as function definition
      if typ == '.' then
        self.group.fun = fun
      else
        self.group.class_fun = fun
      end
      self.group.params = params
      if self.section[#self.section] ~= self.group then
        -- This can happen if we are using a temporary group.
        table.insert(self.section, self.group)
      end
    end
  },
  -- Match anything moves to raw code
  { match = '',
    move = function() return parser.code end,
  }
}

parser.code = {
  { match  = '^%-%- +(.+)$',
    output = function(self, i, d)
      -- Temporary group (not inserted in section).
      self.group = {}
    end,                                   -- replay last line
    move = function() return parser.group, true end,
  },
  { match  = '^%-%-%[%[ *(.*)$',
    output = function(self, i, d)
      self.para = d
      -- Temporary group (not inserted in section).
      self.group = {}
    end,
    move = function() return parser.mgroup end,
  },
}

--=============================================== 

private.output = {}
private.copy_assets = {}

--=============================================== HTML TEMPLATE
function private.output:html(template)
  local tmplt = dub.Template(template or private.HTML_TEMPLATE)
  return tmplt:run {self = self, private = private}
end

function private.copy_assets.html(target)
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

function lib:paraToHtml(para, group, inline)
  if group.math then
    return private.latexImageTag(self, para)
  elseif not inline and string.match(para, '^%*') then
    -- render list
    return private.listToHtml(self, para, group)
  else
    -- filter content
    local p = para
    -- strong
    p = string.gsub(para, '%*([^\n]-)%*', '<strong>%1</strong>')
    -- em
    p = string.gsub(p, '_(.-)_', '<em>%1</em>')
    -- code
    p = string.gsub(p, '@(.-)@', '<code>%1</code>')
    -- auto-link
    p = string.gsub(p, '([a-z]+%.[A-Z][a-z][a-zA-Z]+)([%. ])', "<a href='%1.html'>%1</a>%2")
    -- link
    p = string.gsub(p, '"([^"]+)":([^ %*]+)', function(text, href)
      local chref, trailing = string.match(href, '^(.-)([,%.])$')
      return "<a href='"..(chref or href).."'>"..text.."</a>"..trailing
    end)
    p = string.gsub(p, '%[math%](.-)%[/math%]', function(latex)
      return private.latexImageTag(self, latex)
    end)
    return p
  end
end

function private:listToHtml(para, group)
  local out = '<ul>\n'
  for _, line in ipairs(lk.split(para, '\n')) do
    local depth, text = string.match(line, '^(%*+) (.*)$')
    out = out .. '<li>' .. self:paraToHtml(text, group, true) .. '</li>\n'
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

function private:latexImageTag(latex)
  local target   = self.target
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
    os.exit()
    return mock
  end
  if osTry(string.format('cd %s && dvips base.dvi -E -o base.ps &> /dev/null', tempf)) ~= 0 then
    lk.rmTree(tempf, true)
    os.exit()
    return mock
  end
  if osTry(string.format('cd %s && convert -density 150 base.ps -matte -fuzz 10%% -transparent "#ffffff" base.png', tempf, target, img_name)) ~= 0 then
    lk.rmTree(tempf, true)
    os.exit()
    return mock
  end
  if osTry(string.format('mv %s/base.png %s/latex/%s', tempf, target, img_name)) ~= 0 then
    lk.rmTree(tempf, true)
    os.exit()
    return mock
  end
  lk.rmTree(tempf, true)
  return string.format("<img src='latex/%s'/>", img_name)
end

private.HTML_TEMPLATE = [=[
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>{{self.sections[1].title}}</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta name="description" content="">
    <meta name="author" content="">

    <!-- Le styles -->
    <link href="css/bootstrap.css" rel="stylesheet">
    <link href="css/bootstrap-responsive.css" rel="stylesheet">
    <link href="css/docs.css" rel="stylesheet">
    <!-- TODO ! -->
    <!-- <link href="assets/js/google-code-prettify/prettify.css" rel="stylesheet"> -->
  </head>

  <body>
  <div class="container">

    <div class="row">
      <div class="span3 bs-docs-sidebar">
        <ul class="nav nav-list bs-docs-sidenav affix">
          {% for _, section in ipairs(self.sections) do %}
          <li><a href='#{{section.name}}'><i class='icon-chevron-right'></i> {{section.title}}</a></li>
          {% end %}
        </ul>
      </div>

      <div class="span6">

        {% for _, section in ipairs(self.sections) do %}
        <section id='{{section.name}}'>
          <h2 class='section'>{{section.title}}</h2>

          {% for _, group in ipairs(section) do %}
          {% if group.fun then %}
          <h4 class='method'>{{group.fun}} <code>{{group.params}}</code></h4>
          {% elseif group.class_fun then %}
          <h4 class='function'>{{group.class_fun}} <code>{{group.params}}</code></h4>
          {% end %}

          {% for _, para in ipairs(group) do %}
            {% if group.class then %}
            <p class='{{group.class}}'>
            {% else %}
            <p>
            {% end %}
            {{self:paraToHtml(para, group)}}</p>
          {% end %}
          {% end %}
        </section>
        {% end %}
      </div>

      <div class="span3 bs-docs-sidebar">
        <ul class="nav nav-list bs-docs-sidenav affix">
          {% for _, fullname in ipairs(self.navigation) do %}
          <li{{ fullname == self.fullname and " class='active'" or "" }}><a href='{{fullname}}.html'>{{fullname}}</a></li>
          {% end %}
        </ul>
      </div>
    </div>
  </div>

    <footer class="footer">
      <div class="container">
      </div>
    </footer>

    <!-- Placed at the end of the document so the pages load faster -->
    <!--
    <script src="assets/js/jquery.js"></script>
    <script src="assets/js/bootstrap-transition.js"></script>
    <script src="assets/js/bootstrap-alert.js"></script>
    <script src="assets/js/bootstrap-modal.js"></script>
    <script src="assets/js/bootstrap-dropdown.js"></script>
    <script src="assets/js/bootstrap-scrollspy.js"></script>
    <script src="assets/js/bootstrap-tab.js"></script>
    <script src="assets/js/bootstrap-tooltip.js"></script>
    <script src="assets/js/bootstrap-popover.js"></script>
    <script src="assets/js/bootstrap-button.js"></script>
    <script src="assets/js/bootstrap-collapse.js"></script>
    <script src="assets/js/bootstrap-carousel.js"></script>
    <script src="assets/js/bootstrap-typeahead.js"></script>
    <script src="assets/js/bootstrap-affix.js"></script>
    -->

    
    <script src="http://code.jquery.com/jquery.js"></script>
    <script src="js/bootstrap.min.js"></script>
  </body>
</html>
]=]

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

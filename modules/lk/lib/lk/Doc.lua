--[[------------------------------------------------------

  lk.Doc
  ------

  Documentation generator for lubyk. This generator parses the text using a
  state machine made of current triggers and transitions.

--]]------------------------------------------------------
local lib     = class 'lk.Doc'
local private = {}
local parser  = {}

-- Parse the content of a file given by *path* and return a lk.Doc object 
-- containing the documentation of the class.
function lib.new(path)
  local self = {
    path = path,
    sections = {
   --   {name='create'}, {name='compute'}
    },
    -- This is the 'section' that describes the class itself.
    info = {},
  }
  self.name   = assert(string.match(path, '([^/]+)\.lua$'))
  self.module = string.match(path, '([^/]+)/[^/]+$')
  if self.module then
    self.fullname = self.module .. '.' .. self.name
  else
    self.fullname = self.name
  end

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
  for _, path in ipairs(def.sources) do
    local doc = lib.new(path)
    local trg = def.target .. '/' .. doc.fullname .. '.' .. format
    lk.writeall(trg, output(doc, def.template))
  end
end

function private:parseFile(path)
  local file = assert(io.open(path, "r"))
  local line_i = 0
  local state = parser.start
  for line in file:lines() do
    line_i = line_i + 1
    for i=1,#state do
      local matcher = state[i]
      local m = {string.match(line, matcher.match)}
      if m[1] then
        if matcher.output then
          matcher.output(self, line_i, unpack(m))
        end
        if type(matcher.move) == 'function' then
          state = matcher.move(self)
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

  if state.eof then
    state.eof(self, line_i)
  end
  -- Clean draft content
  self.para  = nil
  self.scrap = nil
  -- Copy name from first section
  self.name = self.sections[1].name
end

--=============================================== Helpers

function private:newSection(title)
  self.group = {class = 'summary'}
  self.section = {self.group}
  table.insert(self.sections, self.section)
  self.section.name = title
end
--=============================================== Doc parser

-- A parser state is defined with:
-- {MATCH_KEY, SUB-STATES, match = function}
-- list of matches, actions
parser.start = {
  -- matchers
  { match  = '^--%[%[.*$',
    move   = {
      -- h1 / h2: new section
      { match  = '^ *(h[12])%. (.+)$',
        output = function(self, i, tag, d)
          private.newSection(self, d)
        end,
        move   = function() return parser.mgroup end,
      },
      { match  = '^--%]',
        output = function(self, i) error('Missing "h1. title" in preamble.') end
      },
    }
  },

  eof = function(self, i) error('Reaching end of document without finding preamble documentation.') end,
}

-- Multi-line documentation
parser.mgroup = {
  -- End of multi-line comment
  { match    = '^--%]',
    move = function() return parser.code end
  },
  -- h1 / h2: new section
  { match = '^ *(h1|h2)%. (.+)$',
    output = function(self, i, tag, d)
      private.newSection(self, d)
    end,
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

parser.code = {
  { match = 'xxx',
  },
}

--=============================================== 

--[[
parser.summary = {
  -- Paragraph = summary
  { match = '^ *([^ ].+)$',
    output = function(doc, i, d) doc.summary = d end,
    { match = '^ *([^ ].+)$',
      output = function(doc, i, d) doc.summary = doc.summary .. ' ' .. d end,
      -- Continue in same
      move = false,
    },
    -- End of paragraph
    { match = '^ *$', 
      output = function(doc, i, d)
        doc.description = {_i = 1 }
        doc._step = ''
      end,
      move = function() return parser.description end,
    },
  },
}

parser.description = {
  -- All text until end of comment = description
  -- End of comment
  { match = '^--%]%]', 
    output = function()
      doc.scrap = ''
    end
    move = function() return parser.code end
  },
  { match = '^ *$',
    output = function(doc, i, d)
      -- next paragraph
      doc._step = ''
      doc.description._i = doc.description._i + 1
    end,
    -- Continue in same
    move = false,
  },
  -- Bullet list
  { match = '^(%*+ [^\n]*)$',
    output = function(doc, i, d)
      local i = doc.description._i
      if doc._step == ' ' then doc._step = '\n' end
      doc.description[i] = (doc.description[i] or '') .. doc._step .. d
      doc._step = ' '
    end,
    -- Continue in same
    move = false,
  },
  { match = '^ *([^\n]*)$',
    output = function(doc, i, d)
      local i = doc.description._i
      doc.description[i] = (doc.description[i] or '') .. doc._step .. d
      doc._step = ' '
    end,
    -- Continue in same
    move = false,
  },
}

parser.code = {
  { match = '^-- *(.*)$',
    output = function(doc, i, d)
      doc.scrap = d
      doc._step = ' '
    end
    move = false,
  },
}

parser.section = {

}
--]]


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

private.HTML_TEMPLATE = [=[
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>{{self.name}}</title>
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

    <!-- Docs nav
    ================================================== -->
    <div class="row">
      <div class="span3 bs-docs-sidebar">
        <ul class="nav nav-list bs-docs-sidenav affix">
          {% for _, section in ipairs(self.sections) do %}
          <li><a href='#{{section.name}}'><i class='icon-chevron-right'></i> {{section.name}}</a></li>
          {% end %}
        </ul>
      </div>

      <div class="span9">

        {% for _, section in ipairs(self.sections) do %}
        <section id='{{section.name}}'>
          <div class="page-header">
            <h1>{{section.name}}</h1>
          </div>

          {% for _, group in ipairs(section) do %}
          {% for _, para in ipairs(group) do %}
            {% if group.class then %}
            <p class='{{group.class}}'>{{para}}</p>
            {% else %}
            <p>{{para}}</p>
            {% end %}
          {% end %}
          {% end %}

          <h3>open <code>(path)</code></h3>
          <p>Looking at just the dropdown menu, here's the required HTML. You need to wrap the dropdown's trigger and the dropdown menu within <code>.dropdown</code>, or another element that declares <code>position: relative;</code>. Then just create the menu.</p>

            
          <h3>isHot <code>(transition, morph)</code></h3>
          <p>Looking at just the dropdown menu, here's the required HTML. You need to wrap the dropdown's trigger and the dropdown menu within <code>.dropdown</code>, or another element that declares <code>position: relative;</code>. Then just create the menu.</p>

          <p>Looking at just the dropdown menu, here's the required HTML. You need to wrap the dropdown's trigger and the dropdown menu within <code>.dropdown</code>, or another element that declares <code>position: relative;</code>. Then just create the menu.</p>

<pre class="prettyprint linenums">
&lt;div class="dropdown"&gt;
  &lt;!-- Link or button to toggle dropdown --&gt;
  &lt;ul class="dropdown-menu" role="menu" aria-labelledby="dLabel"&gt;
    &lt;li&gt;&lt;a tabindex="-1" href="#"&gt;Action&lt;/a&gt;&lt;/li&gt;
    &lt;li&gt;&lt;a tabindex="-1" href="#"&gt;Another action&lt;/a&gt;&lt;/li&gt;
    &lt;li&gt;&lt;a tabindex="-1" href="#"&gt;Something else here&lt;/a&gt;&lt;/li&gt;
    &lt;li class="divider"&gt;&lt;/li&gt;
    &lt;li&gt;&lt;a tabindex="-1" href="#"&gt;Separated link&lt;/a&gt;&lt;/li&gt;
  &lt;/ul&gt;
&lt;/div&gt;
</pre>
        </section>
        {% end %}
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

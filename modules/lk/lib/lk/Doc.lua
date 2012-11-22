--[[------------------------------------------------------

  lk.Doc
  ------

  Documentation generator for lubyk. This generator parses the text using a
  state machine made of current triggers and transitions.

--]]------------------------------------------------------
local lib = {}
local private = {}
lib.__index = lib
lk.Doc = lib

setmetatable(lib, {__call = function(lib, ...) return lib.new(...) end})

-- Parse the content of a file given by *path* and return a lk.Doc object 
-- containing the documentation of the class.
function lib.new(path)
  local self = {path = path}
  setmetatable(self, lib)
  return private.parseFile(self, path)
end

local parser = {}

-- A parser state is defined with:
-- {MATCH_KEY, SUB-STATES, key = function}
-- list of matches, actions
parser.start = {
  -- MATCH KEY not used for start
  key = '', 
  -- sub-matchers
  -- actions
  eof = function(doc, line) error('Reaching end of document without finding preamble documentation.') end,

  --- PREAMBLE
  { key = '^--%[%[.*$',
    -- no action

    -- matchers
    { key    = '^ *h1. (.*)$', 
      output = function(doc, line, title) doc.title = title end,
      move   = function() return parser.summary end,
    },
    { key    = '^--%]',
      output = function(doc, line) error('Missing h1 title in preamble (line '.. line .. ').') end,
    },
  },
}

parser.summary = {
  -- Paragraph = summary
  { key = '^ *([^ ].+)$',
    output = function(doc, i, d) doc.summary = d end,
    { key = '^ *([^ ].+)$',
      output = function(doc, i, d) doc.summary = doc.summary .. ' ' .. d end,
      -- Continue in same
      move = false,
    },
    -- End of paragraph
    { key = '^ *$', 
      output = function(doc, i, d)
        doc.description = ''
        doc._step = ''
      end,
      move = function() return parser.description end,
    },
  },
}

parser.description = {
  -- All text until end of comment = description
  -- End of comment
  { key = '^--%]%]', 
    move = function() return parser.section end
  },
  { key = '^ *$',
    output = function(doc, i, d) doc._step = '\n\n' end,
    -- Continue in same
    move = false,
  },
  { key = '^ *([^\n]*)$',
    output = function(doc, i, d)
      doc.description = doc.description .. doc._step .. d
      doc._step = ' '
    end,
    -- Continue in same
    move = false,
  },
}

parser.section = {
}

function private:parseFile(path)
  local file = assert(io.open(path, "r"))
  local line_i = 0
  local state = parser.start
  local doc = {}
  for line in file:lines() do
    line_i = line_i + 1
    for i=1,#state do
      local sub = state[i]
      local m = {string.match(line, sub.key)}
      if m[1] then
        if sub.output then
          sub.output(doc, line_i, unpack(m))
        end
        if type(sub.move) == 'function' then
          state = sub.move()
        elseif sub.move == false then
          -- do not change state
        else
          state = sub.move or sub
        end
        break
      end
    end
  end

  if state.eof then
    state.eof(doc, line_i)
  end
  return doc
end

function private:match()
end

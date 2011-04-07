--[[------------------------------------------------------

  editor.Library
  --------------

  Database of node objects that can be used in patches. The
  Library can contain many sources patterns (folders) that
  are parsed to update Library content.

--]]------------------------------------------------------

local lib      = {type='editor.Library'}
lib.__index    = lib
editor.Library = lib

-- Prepare the database for events
local function prepare_db(self)
  local db = self.db
  -- FIXME: only create tables if db tables do not exist yet

  -- TODO: store hue

  if not false then
    db:exec[[
      CREATE TABLE nodes (id INTEGER PRIMARY KEY, name TEXT, path TEXT, code TEXT, keywords TEXT);
      CREATE INDEX nodes_keywords_idx ON nodes(keywords);
      CREATE UNIQUE INDEX nodes_id_idx ON nodes(id);
      CREATE UNIQUE INDEX nodes_name_idx ON nodes(name);
    ]]
  end

  ------------------------------------------------------------  READ nodes
  self.get_node_by_position_and_filter_stmt = db:prepare[[
    SELECT * FROM nodes WHERE keywords LIKE :filter ORDER BY name LIMIT 1 OFFSET :p;
  ]]

  self.get_node_count_with_filter_stmt = db:prepare[[
    SELECT COUNT(*) FROM nodes WHERE keywords LIKE :filter;
  ]]

  ------------------------------------------------------------  WRITE nodes
  self.add_node_stmt = db:prepare[[
    INSERT INTO nodes VALUES (NULL, :name, :path, :code, :keywords);
  ]]
end

local function addNode(self, lib_name, filepath)
  local name = lib_name .. '.' .. string.match(filepath, '([^%./]+)%.lua$')
  local stmt = self.add_node_stmt
  local code = lk.readall(filepath)
  -- TODO: extract information from first comment such as
  -- 'keywords', 'hue', 'author', 'help', etc.
  stmt:bind_names {
    name = name,
    path = filepath,
    code = lk.readall(filepath),
    keywords = name,
  }
  stmt:step()
  stmt:reset()
end

setmetatable(lib, {
  --- Create a new editor.Link reflecting the content of a remote
  -- link. If the process view is not shown, the LinkView is not
  -- created.
 __call = function(table, db, delegate)
  local self = {
    -- delegate used for drag&drop operations in LinkView
    delegate = delegate,
    -- Dir patterns to glob for files.
    sources = {
      lubyk = _lubyk_settings.lubyk_lib_path .. '/lua/lubyk',
    },
  }
  if db then
    self.db = db
  else
    self.filepath = _lubyk_settings.editor.library_path or
                    string.format('%s/.lubyk/editor/library.sqlite3', os.getenv('HOME'))
    lk.makePath(lk.directory(self.filepath))
    self.db = sqlite3.open(self.filepath)
  end
  prepare_db(self)
  if _lubyk_settings.editor.library_sources then
    for lib_name, path in pairs(_lubyk_settings.editor.library_sources) do
      self.sources[lib_name] = path
    end
  end
  setmetatable(self, lib)
  return self
end})

function lib:addSource(lib_name, path)
  self.sources[lib_name] = path
end

-- Recreate database from content in filesystem.
function lib:sync()
  local db = self.db
  db:exec[[
    TRUNCATE nodes;
  ]]
  for lib_name, path in pairs(self.sources) do
    local dir = lk.Dir(path)
    for file in dir:glob('[.]lua$') do
      addNode(self, lib_name, file)
    end
  end
end

function lib:nodeCount(filter)
  local stmt = self.get_node_count_with_filter_stmt
  stmt:bind_names {filter = filter or '%'}
  return stmt:first_row()[1]
end

function lib:node(pos, filter)
  local stmt = self.get_node_by_position_and_filter_stmt
  stmt:bind_names {
    p = (pos or 1) - 1,
    filter = '%' .. (filter or '') .. '%',
  }
  local row = stmt:first_row()
  return {
    name = row[1],
    name = row[2],
    path = row[3],
    code = row[4],
    keywords = row[5],
  }
end

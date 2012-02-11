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
local private  = {}

setmetatable(lib, {
  --- Create a new editor.Link reflecting the content of a remote
  -- link. If the process view is not shown, the LinkView is not
  -- created.
  -- 'table_name' can be 'prototypes' or 'controls'
 __call = function(lib, table_name, db)
   table_name = table_name or 'prototype'
  local self = {
    -- Dir patterns to glob for files.
    sources = {},
    -- This is the name of the table containing the objects in the
    -- database. It is also used to get the sources from editor
    -- Settings.
    table_name = table_name,
  }
  if db then
    self.db = db
  else
    self.filepath = editor.Settings.db_path
    lk.makePath(lk.directory(self.filepath))
    self.db = sqlite3.open(self.filepath)
  end
  private.prepareDb(self)
  private.setupSources(self)

  setmetatable(self, lib)
  return self
end})

function lib:addSource(lib_name, path)
  self.sources[lib_name] = path
end

-- Recreate database from content in filesystem.
function lib:sync()
  local db = self.db
  db:exec(private.gsub('DELETE from NODE_TABLE;', 'NODE_TABLE', self.table_name))
  for _, dir in ipairs(self.sources) do
    for folder in dir:list() do
      if lk.fileType(folder) == 'directory' then
        local _, lib_name = lk.directory(folder)
        local dir = lk.Dir(folder)
        for file in dir:glob('[.]lua$') do
          private.addNode(self, lib_name, file)
        end
      end
    end
  end
end

function lib:nodeCount(filter)
  filter = private.prepareFilter(filter)
  local stmt = self.get_node_count_with_filter_stmt
  stmt:bind_names {filter = filter}
  return stmt:first_row()[1]
end

function lib:code(name)
  local stmt = self.get_code_by_name_stmt
  stmt:bind_names {name = name}
  local row = stmt:first_row()
  if row then
    return row[1]
  else
    return nil
  end
end

function lib:node(filter, pos)
  if type(filter) == 'number' then
    pos = filter
    filter = nil
  end
  filter = private.prepareFilter(filter)
  local stmt = self.get_node_by_position_and_filter_stmt
  local filter = private.prepareFilter(filter)
  stmt:bind_names {
    p = (pos or 1) - 1,
    filter = filter,
  }
  local row = stmt:first_row()
  if row then
    return {
      --id   = row[1],
      name = row[2],
      path = row[3],
      code = row[4],
      keywords = row[5],
    }
  else
    return nil
  end
end

--=============================================== PRIVATE

local function gsub(str, pat, rep)
  local str = string.gsub(str, pat, rep)
  -- Avoid returning substitution count values 
  return str
end
private.gsub = gsub

function private:setupSources()
  -- prototypes_base_src, controls_base_src
  local list = editor.Settings[self.table_name .. '_base_src']
  if list then
    -- Use the real list in case we have a copy-on-write (empty) placeholder
    -- list.
    list = list._placeholder or list
    for _, path in ipairs(list) do
      local dir = lk.Dir(Lubyk.lib .. '/' .. path)
      table.insert(self.sources, dir)
    end
  end

  -- prototypes_src, controls_src
  list = editor.Settings[self.table_name .. '_src']
  if list then
    -- Use the real list in case we have a copy-on-write (empty) placeholder
    -- list.
    list = list._placeholder or list
    for _, path in ipairs(list) do
      local dir = lk.Dir(path)
      table.insert(self.sources, dir)
    end
  end
end

-- Prepare the database for events
function private:prepareDb()
  local db = self.db
  -- FIXME: only create tables if db tables do not exist yet

  -- Code is not used for controls.
  if not false then
    db:exec(gsub([[
      CREATE TABLE NODE_TABLE (id INTEGER PRIMARY KEY, name TEXT, path TEXT, code TEXT, keywords TEXT);
      CREATE INDEX NODE_TABLE_keywords_idx ON NODE_TABLE(keywords);
      CREATE UNIQUE INDEX NODE_TABLE_id_idx ON NODE_TABLE(id);
      CREATE UNIQUE INDEX NODE_TABLE_name_idx ON NODE_TABLE(name);
    ]], 'NODE_TABLE', self.table_name))
  end

  ------------------------------------------------------------  READ nodes
  self.get_node_by_position_and_filter_stmt = db:prepare(gsub([[
    SELECT * FROM NODE_TABLE WHERE keywords LIKE :filter ORDER BY name LIMIT 1 OFFSET :p;
  ]], 'NODE_TABLE', self.table_name))

  self.get_code_by_name_stmt = db:prepare(gsub([[
    SELECT code FROM NODE_TABLE WHERE name = :name LIMIT 1;
  ]], 'NODE_TABLE', self.table_name))

  self.get_node_count_with_filter_stmt = db:prepare(gsub([[
    SELECT COUNT(*) FROM NODE_TABLE WHERE keywords LIKE :filter;
  ]], 'NODE_TABLE', self.table_name))

  ------------------------------------------------------------  WRITE nodes
  self.add_node_stmt = db:prepare(gsub([[
    INSERT INTO NODE_TABLE VALUES (NULL, :name, :path, :code, :keywords);
  ]], 'NODE_TABLE', self.table_name))
end

function private:addNode(lib_name, filepath)
  local name = lib_name .. '.' .. string.match(filepath, '([^%./]+)%.lua$')
  local stmt = self.add_node_stmt
  local code = lk.readall(filepath)
  -- TODO: extract information from first comment such as
  -- 'keywords', 'hue', 'author', 'help', etc.
  stmt:bind_names {
    name = name,
    path = filepath,
    code = lk.readall(filepath),
    keywords = lib_name ..'.'.. name,
  }
  stmt:step()
  stmt:reset()
end

function private.prepareFilter(filter)
  if not filter then
    return '%'
  elseif not string.match(filter, '%%') then
    return '%' .. (filter or '') .. '%'
  else
    return filter
  end
end

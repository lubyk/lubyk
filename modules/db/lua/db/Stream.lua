--[[------------------------------------------------------

  db.Stream
  ---------

  A simple database wrapper for streaming data (tracks with
  timecodes). The actual data is stored with msgpack in an
  SQLite3 database.

--]]------------------------------------------------------
require 'sqlite3'

local lib = {}
lib.__index = lib
db.Stream = lib

-- Execute the given function in a transaction.
local function transaction(db, func)
  db:exec('BEGIN;')
    func()
  db:exec('COMMIT;')
end

-- Prepare the database for events
local function prepare_db(self, ...)
  local db = sqlite3.open_memory() -- open(...)
  self.db = db
  transaction(db, function()
    db:exec[[
      CREATE TABLE events (id INTEGER PRIMARY KEY, t REAL);
      CREATE UNIQUE INDEX evetns_idx ON events(t);

      CREATE TABLE tracks (id INTEGER PRIMARY KEY, name TEXT);
      CREATE TABLE data   (event_id INTEGER, track_id INTEGER, value REAL);
      CREATE UNIQUE INDEX data_idx ON data(event_id, track_id);
    ]]
  end)
  self.get_event_stmt                = db:prepare[[ SELECT id FROM events WHERE t = :t ]]
  self.add_event_stmt                = db:prepare[[ INSERT INTO events VALUES (NULL, :t) ]]
  self.get_next_event_stmt           = db:prepare[[ SELECT id, t FROM events WHERE t > :t ORDER BY t ASC LIMIT 1 ]]
  self.get_next_event_for_track_stmt = db:prepare[[ SELECT id, t FROM events INNER JOIN data ON data.event_id = events.id WHERE t > :t AND track_id = :track_id ORDER BY t ASC LIMIT 1 ]]

  self.get_track_name_stmt           = db:prepare[[ SELECT name FROM tracks WHERE id = :id ]]
  self.get_track_id_stmt             = db:prepare[[ SELECT id FROM tracks WHERE name = :name ]]
  self.add_track_stmt                = db:prepare[[ INSERT INTO tracks VALUES (NULL, :name) ]]

  self.get_data_event_stmt           = db:prepare[[ SELECT tracks.name, value FROM data INNER JOIN tracks ON tracks.id = track_id WHERE event_id = :id ]]
  self.get_data_stmt                 = db:prepare[[ SELECT tracks.name, value FROM data INNER JOIN tracks ON tracks.id = track_id INNER JOIN events ON events.id == event_id WHERE events.t = :t ]]
  self.get_data_track_stmt           = db:prepare[[ SELECT value FROM data INNER JOIN tracks ON tracks.id = track_id INNER JOIN events ON events.id == event_id WHERE events.t = :t AND tracks.name = :track ]]
  self.add_or_update_data_stmt       = db:prepare[[ INSERT OR REPLACE INTO data VALUES (:event_id, :track_id, :value) ]]
end

local function get_event_id(self, t)
  local stmt = self.get_event_stmt
  stmt:bind_names{t = t}
  local row = stmt:first_row()
  if row then
    return row[1]
  else
    return nil
  end
end

-- Add a new event. Returns new event id.
local function add_event(self, t)
  local stmt = self.add_event_stmt
  stmt:bind_names{t = t}
  stmt:step()
  stmt:reset()
  return self.db:last_insert_rowid()
end

-- Find a track name from a given track id.
-- The track name is cached in memory.
local function get_track_name(self, id)
  local name = self.track_name_from_id[id]
  if name then
    return name
  end
  local stmt = self.get_track_name_stmt
  stmt:bind_names{id = id}
  local row = stmt:first_row()
  if row then
    self.track_name_from_id[id] = row[1]
    self.track_id_from_name[row[1]] = id
    return row[1]
  else
    return nil
  end
end

-- Add a new track. Returns new track id.
local function add_track(self, name)
  local stmt = self.add_track_stmt
  stmt:bind_names{name = name}
  stmt:step()
  stmt:reset()
  local id = self.db:last_insert_rowid()
  self.track_name_from_id[id] = name
  self.track_id_from_name[name] = id
  return id
end

-- Find a track id from a given track name. Creates a new track if name
-- is not found.
-- The track id is cached in memory.
local function get_track_id(self, name, should_create)
  local id = self.track_id_from_name[name]
  if id then
    return id
  end
  local stmt = self.get_track_id_stmt
  stmt:bind_names{name = name}
  local row = stmt:first_row()
  if row then
    self.track_id_from_name[name] = row[1]
    self.track_name_from_id[row[1]] = name
    return row[1]
  elseif should_create then
    return add_track(self, name)
  else
    return nil
  end
end

local function get_data_with_event_id(self, event_id)
  local stmt = self.get_data_event_stmt
  local res = {}
  stmt:bind_names{id = event_id}
  for row in stmt:rows() do
    res[row[1]] = row[2]
  end
  return res
end

-- Add a new event. Returns new event id.
local function add_or_update_data(self, event_id, track_id, value)
  local stmt = self.add_or_update_data_stmt
  stmt:bind_names{event_id = event_id, track_id = track_id, value = value}
  stmt:step()
  stmt:reset()
  --return self.db:last_insert_rowid()
end

-- Get values for all tracks for this event.
-- @returns nil if the event does not exist.
local function get_data(self, t)
  local stmt = self.get_data_stmt
  local res = {}
  local has_values = false
  stmt:bind_names{t = t}
  for row in stmt:rows() do
    has_values = true
    -- tracks.name, value
    res[row[1]] = row[2]
  end
  if has_values then
    return res
  else
    return nil
  end
end

-- Get values for all tracks for this event.
-- @returns nil if the event does not exist.
local function get_data_for_track(self, t, track_name)
  local stmt = self.get_data_track_stmt
  local res = {}
  local has_values = false
  stmt:bind_names{t = t, track = track_name}
  local row = stmt:first_row()
  if row then
    -- value
    return row[1]
  else
    return nil
  end
end

local function get_next_event(self, t)
  local stmt = self.get_next_event_stmt
  stmt:bind_names{t = t}
  local row = stmt:first_row()
  if row then
    self.next_id = row[1]
    self.next_t  = row[2]
    return row[2]
  else
    return nil
  end
end

local function get_next_event_for_track(self, t, track_name)
  local stmt = self.get_next_event_for_track_stmt
  local track_id = get_track_id(self, track_name, false)
  if not track_id then
    return nil
  end
  stmt:bind_names{t = t, track_id = track_id}
  local row = stmt:first_row()
  if row then
    self.next_id = row[1]
    self.next_t  = row[2]
    return row[2]
  else
    return nil
  end
end

setmetatable(lib, {
  -- new method
 __call = function(table, ...)
  local instance = {track_name_from_id = {}, track_id_from_name = {}, last_t = 0, last_id = nil, next_t = 0, next_id = nil}
  prepare_db(instance)
  setmetatable(instance, lib)
  return instance
end})


function lib:set(hash)
  transaction(self.db, function()
    local t = hash.t
    local last_t = self.last_t
    local event_id
    local existing_data = nil
    -- 1. is this a new event ?
    if t > last_t then
      -- ordered event insertion optimization
      -- new event
      event_id = add_event(self, t)
      self.last_t  = t
      self.last_id = event_id
    elseif t == last_t then
      -- same event
      event_id = self.last_id
    else
      -- existing or new event
      event_id = get_event_id(self, t)
      if not event_id then
        -- create
        event_id = add_event(self, v)
      end
    end
    -- create or update: insert values
    for track_name, value in pairs(hash) do
      if track_name ~= 't' then
        add_or_update_data(self, event_id, get_track_id(self, track_name, true), value)
      end
    end
  end)
end

function lib:next(t, track_name)
  if track_name then
    return get_next_event_for_track(self, t, track_name)
  else
    return get_next_event(self, t)
  end
end

function lib:at(t, track_name)
  if track_name then
    return get_data_for_track(self, t, track_name)
  else
    return get_data(self, t)
  end
end

function lib:isopen()
  return self.db:isopen()
end

function lib:close()
  return self.db:close()
end


function lib:debug()
  local db = self.db
  print('--------------------- events')
  for r in db:nrows("select * from events") do
    print(r.id, r.t)
  end

  print('--------------------- tracks')
  for r in db:nrows("select * from tracks") do
    print(r.id, r.name)
  end

  print('--------------------- data')
  for r in db:nrows("SELECT events.t, tracks.name, value FROM data INNER JOIN tracks ON tracks.id = track_id INNER JOIN events ON events.id == event_id") do
    print(r.t, r.name, r.value)
  end
end

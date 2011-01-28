--[[------------------------------------------------------

  db.Stream
  ---------

  A simple database wrapper for streaming data (tracks with
  timecodes). The actual data is stored with msgpack in an
  SQLite3 database.

--]]------------------------------------------------------
require 'sqlite3'

----------------------------------------------------------
----             PRIVATE                              ----
----------------------------------------------------------

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
      CREATE TABLE data   (event_id INTEGER, track_id INTEGER, value);
      CREATE UNIQUE INDEX data_idx ON data(event_id, track_id);
    ]]
  end)
  self.get_event_stmt                = db:prepare[[ SELECT id FROM events WHERE t = :t ]]
  self.add_event_stmt                = db:prepare[[ INSERT INTO events VALUES (NULL, :t) ]]
  self.get_next_event_stmt           = db:prepare[[ SELECT t FROM events WHERE t > :t ORDER BY t ASC LIMIT 1 ]]

  self.get_track_id_stmt             = db:prepare[[ SELECT id FROM tracks WHERE name = :name ]]
  self.add_track_stmt                = db:prepare[[ INSERT INTO tracks VALUES (NULL, :name) ]]

  self.add_or_update_data_stmt       = db:prepare[[ INSERT OR REPLACE INTO data VALUES (:event_id, :track_id, :value) ]]

  -- all data at event t
  self.get_data_stmt = db:prepare[[
    SELECT tracks.name, value
    FROM data
      INNER JOIN tracks ON tracks.id = track_id
      INNER JOIN events ON events.id == event_id
    WHERE
      events.t = :t
  ]]

  --------------------------------------------------------- db.Track queries
  self.get_next_event_for_track_stmt = db:prepare[[
    SELECT t
    FROM events
      INNER JOIN data ON data.event_id = events.id
    WHERE
      t > :t AND
      track_id = :id
    ORDER BY t ASC
    LIMIT 1
  ]]

  -- all data in rage [t1, t2[
  self.get_data_range_track_stmt = db:prepare[[
    SELECT events.t, value
    FROM data
      INNER JOIN events ON events.id == event_id
    WHERE
      events.t >= :t1 AND events.t < :t2 AND
      track_id = :id
    ORDER BY events.t ASC
  ]]

  self.get_data_track_stmt = db:prepare[[
    SELECT value
    FROM data
      INNER JOIN events ON events.id == event_id
    WHERE
      events.t = :t AND
      track_id = :id ]]
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

-- Add a new track. Returns new track id.
local function add_track(self, name)
  local stmt = self.add_track_stmt
  stmt:bind_names{name = name}
  stmt:step()
  stmt:reset()
  local id = self.db:last_insert_rowid()
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
    return row[1]
  elseif should_create then
    return add_track(self, name)
  else
    return nil
  end
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
  local res = {t = t}
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
-- @returns an iterator over a list of rows or nil if nothing is found.
local function get_data_range_for_track_id(self, t1, t2, track_id)
  local stmt = self.get_data_range_track_stmt
  stmt:bind_names{t1 = t1, t2 = t2, id = track_id}
  return stmt:rows()
end

-- Get values for all tracks for this event.
-- @returns nil if the event does not exist.
local function get_data_for_track_id(self, t, track_id)
  local stmt = self.get_data_track_stmt
  local res = {}
  local has_values = false
  stmt:bind_names{t = t, id = track_id}
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
    return row[1]
  else
    return nil
  end
end

local function get_next_event_for_track(self, t, track_id)
  local stmt = self.get_next_event_for_track_stmt
  stmt:bind_names{t = t, id = track_id}
  local row = stmt:first_row()
  if row then
    return row[1]
  else
    return nil
  end
end

----------------------------------------------------------
----             PUBLIC  db.Stream                    ----
----------------------------------------------------------

local lib = {}
lib.__index = lib
db.Stream = lib

setmetatable(lib, {
  -- new method
 __call = function(table, ...)
  local instance = {tracks = {},
    track_id_from_name = {},
    last_t = 0,
    last_id = nil,
    -- playback
    playing = false,
    -- playback head
    t = 0,
    loop = false,
    -- recording
    recording = false,
    -- recoding start time (offset)
    rec_offset = 0
  }
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
        event_id = add_event(self, t)
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

-- Record incoming data with the current time. This
-- method only records after rec_start() has been
-- called (while recording is true).
function lib:rec(hash)
  if self.recording then
    hash.t = worker:now() - self.rec_offset
    self:set(hash)
  end
end

function lib:rec_start()
  self.rec_offset = worker:now()
  self.recording  = true
end

function lib:rec_stop()
  self.recording = false
end

function lib:play(t)
  if not self.playback then
    error("Cannot play without a playback function.")
  end

  if not t then
    -- default start is self.t (current head position)
    t = self.t
  end

  self.playing = true

  -- 10 = default timer value will be updated after first call
  self.playback_timer = rk.Timer(10, function()
    -- triggered right after start (no delay)
    local data = self:at(t)
    if data then
      -- move head to where we are playing
      self.t = t
      self.playback(data)
    end

    local next_t = self:next(t)

    if not next_t and self.loop then
      t = 0
      next_t = self:next(-1)
    end

    if next_t then
      local sleep_t = next_t - t
      t = next_t
      return sleep_t
    else
      self.playing = false
      -- move head back to 0 after everything is played
      self.t = 0
      return 0 -- stop
    end
  end)
  self.playback_timer:start()
end

function lib:stop()
  if self.playback_timer then
    self.playback_timer:stop()
    self.playback_timer = nil
    self.playing = false
  end
end

function lib:first()
  return self:next(-1)
end

function lib:next(t)
  return get_next_event(self, t)
end

-- Find values at a given point in time. The returned hash contains the values with
-- the event time 't'.
function lib:at(t)
  return get_data(self, t)
end

function lib:is_open()
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

function lib:track(track_name)
  local track = self.tracks[track_name]
  if not track then
    track = db.Track(self, track_name, get_track_id(self, track_name, true))
    self.tracks[track_name] = track
  end
  return track
end

----------------------------------------------------------
----             PUBLIC  db.Track                     ----
----------------------------------------------------------

local lib = {}
lib.__index = lib
db.Track = lib

setmetatable(lib, {
  -- new method
 __call = function(table, stream, track_name, track_id)
  local instance = {db = stream.db, stream = stream, name = track_name, id = track_id}
  setmetatable(instance, lib)
  return instance
end})

function lib:at(t)
  return get_data_for_track_id(self.stream, t, self.id)
end

-- Return events in range [t1, t2[
function lib:range(t1, t2)
  return get_data_range_for_track_id(self.stream, t1, t2, self.id)
end

function lib:next(t)
  return get_next_event_for_track(self.stream, t, self.id)
end

function lib:first()
  return self:next(-1)
end

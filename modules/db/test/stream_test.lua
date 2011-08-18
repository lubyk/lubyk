--[[------------------------------------------------------

  db.Stream test
  --------------

  The Stream database is made for recording streaming data
  with timestamps. The data is organized in tracks with
  unique names such as "x", "y", etc.

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('db.Stream')

ROWS = {
  [10] = {t=10, a=1.1, b=2, c=3},
  [12] = {t=12, a=3},
  [15] = {t=15, a=5, b=3}
}

local function setupBasicData(t)
  t.stream = db.Stream('tmp.db')
  -- set data at event time '10'
  t.stream:set{t=10, a=1.1, b=2.0}
  t.stream:set{t=10, c=3}
  t.stream:set(ROWS[12])
  t.stream:set(ROWS[15])
end

function should.teardown(t)
  t.stream:close()
  t.stream = nil
end

function should.open(t)
  t.stream = db.Stream('tmp2.db')
  assertTrue(t.stream:is_open())
end

function should.getFirstTimestamp(t)
  setupBasicData(t)
  -- get first event time
  assertEqual(10, t.stream:first())
  assertEqual(10, t.stream:first())
end

function should.writeStrings(t)
  setupBasicData(t)
  t.stream:set({t=25, name='hello'})
  assertTableEqual({t=25, name='hello'}, t.stream:at(25))
end

function should.getNextTimestamp(t)
  setupBasicData(t)
  -- get first event time
  assertEqual(10, t.stream:next(0))
  -- get next event time
  assertEqual(12, t.stream:next(10))
  -- no next event after 15
  assertNil(t.stream:next(15))
end

function should.getDataWithAt(t)
  setupBasicData(t)
  -- get all data at event time '10'
  assertTableEqual({a=1.1, b=2, c=3}, t.stream:at(10))
  -- get all data at event time '15'
  assertTableEqual({a=5, b=3}, t.stream:at(15))
  -- no data at event time '123'
  assertNil(t.stream:at(123))
end

function should.record(t)
  t.stream = db.Stream('tmp.db')
  local now = worker:now()
  t.stream:recStart()
  assertTrue(t.stream.recording)
  assertEqual(now, t.stream.rec_offset)
  t.stream.recording = true
  t.stream:rec({a = 1})
  worker:sleep(10)
  t.stream:rec({a = 1})
  worker:sleep(10)
  t.stream:rec({a = 1})
  local first_event = t.stream:first()
  assertInRange(first_event + 10, first_event + 12, t.stream:next(first_event))
end

function should.play(t)
  setupBasicData(t)
  local start = worker:now()
  function t.stream.playback(row)
    assertValueEqual(ROWS[row.t], row)
    assertInRange(start + row.t, start + row.t + 3, worker:now())
  end
  t.stream:play()
  while t.stream.playing do
    worker:sleep(25)
  end
  assertFalse(t.stream.playing)
end

function should.notDrift(t)
  setupBasicData(t)
  for i = 1,100 do
    local j = i * 10 + math.floor(math.random() * 5)
    t.stream:set({t = j, a = i})
  end
  local now = worker:now()
  function t.stream.playback(row)
    -- no cumulated drift
    assertInRange(now + row.t, now + row.t + 3, worker:now())
  end
  t.stream:play()
  while t.stream.playing do
    worker:sleep(10)
  end
  assertFalse(t.stream.playing)
end

function should.getTrack(t)
  setupBasicData(t)
  local track = t.stream:track('b')
  assertEqual('b', track.name)
  -- always return the same track object
  assertTrue(t.stream:track('b') == track)
  assertEqual(2, track.id)
end

function should.getFirstTimestamp(t)
  setupBasicData(t)
  local track = t.stream:track('d')
  t.stream:set({t=22, d=5})
  -- get first event time
  assertEqual(22, track:first())
end

function should.getNextTrackTimestamp(t)
  setupBasicData(t)
  local track = t.stream:track('b')
  -- get first event time
  assertEqual(10, track:first())
  -- get next event time
  assertEqual(15, track:next(10))
  -- no next event after 15
  assertNil(track:next(15))
end

function should.getTrackDataAt(t)
  setupBasicData(t)
  local track = t.stream:track('b')
  -- get data at event time '10' for track 'b'
  assertEqual(ROWS[10].b, track:at(10))
  -- get all data at event time '12'
  assertEqual(ROWS[15].b, track:at(15))
  -- no data at event time '123'
  assertNil(track:at(12))
end

function should.getDataRange(t)
  setupBasicData(t)
  local track = t.stream:track('a')
  -- get all data in range [a,b[
  for row in track:range(10,15) do
    if row[1] == 10 then
      assertEqual(ROWS[10].a, row[2])
    elseif row[1] == 12 then
      assertEqual(ROWS[12].a, row[2])
    else
      -- error
      assertEqual(nil, row[1])
    end
  end
  -- last event only
  for row in track:range(15,15) do
    -- should not return any row: range means [a, b[ (b not included)
    assertTrue(false)
  end
end

test.all()

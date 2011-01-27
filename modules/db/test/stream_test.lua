--[[------------------------------------------------------

  db.Stream test
  --------------

  The Stream database is made for recording streaming data
  with timestamps. The data is organized in tracks with
  unique names such as "x", "y", etc.

--]]------------------------------------------------------
require 'rubyk'

local should = test.Suite('db.Stream')

ROWS = {
  [10] = {t=10, a=1.1, b=2, c=3},
  [12] = {t=12, a=3},
  [15] = {t=15, a=5, b=3}
}

local function setup_basic_data(t)
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
  assert_true(t.stream:is_open())
end

function should.get_first_timestamp(t)
  setup_basic_data(t)
  -- get first event time
  assert_equal(10, t.stream:first())
  assert_equal(10, t.stream:first())
end

function should.get_next_timestamp(t)
  setup_basic_data(t)
  -- get first event time
  assert_equal(10, t.stream:next(0))
  -- get next event time
  assert_equal(12, t.stream:next(10))
  -- no next event after 15
  assert_nil(t.stream:next(15))
end

function should.get_data_with_at(t)
  setup_basic_data(t)
  -- get all data at event time '10'
  assert_equal({a=1.1, b=2, c=3}, t.stream:at(10))
  -- get all data at event time '15'
  assert_equal({a=5, b=3}, t.stream:at(15))
  -- no data at event time '123'
  assert_nil(t.stream:at(123))
end

function should.record(t)
  t.stream = db.Stream('tmp.db')
  local now = worker:now()
  t.stream:rec({a = 1})
  worker:sleep(10)
  t.stream:rec({a = 1})
  worker:sleep(10)
  t.stream:rec({a = 1})
  local first_event = t.stream:first()
  assert_in_range(first_event + 10, first_event + 12, t.stream:next(first_event))
end

function should.play(t)
  setup_basic_data(t)
  local start = worker:now()
  t.stream:play(function(row)
    assert_equal(ROWS[row.t], row)
    assert_in_range(start + row.t, start + row.t + 3, worker:now())
  end)
  while t.stream.playing do
    worker:sleep(25)
  end
  assert_false(t.stream.playing)
end

function should.not_drift(t)
  setup_basic_data(t)
  for i = 1,100 do
    local j = i * 10 + math.floor(math.random() * 5)
    t.stream:set({t = j, a = i})
  end
  local now = worker:now()
  t.stream:play(function(row)
    -- no cumulated drift
    assert_in_range(now + row.t, now + row.t + 3, worker:now())
  end)
  while t.stream.playing do
    worker:sleep(10)
  end
  assert_false(t.stream.playing)
end

function should.get_track(t)
  setup_basic_data(t)
  local track = t.stream:track('b')
  assert_equal('b', track.name)
  -- always return the same track object
  assert_true(t.stream:track('b') == track)
  assert_equal(2, track.id)
end

function should.get_first_timestamp(t)
  setup_basic_data(t)
  local track = t.stream:track('d')
  t.stream:set({t=22, d=5})
  -- get first event time
  assert_equal(22, track:first())
end

function should.get_next_track_timestamp(t)
  setup_basic_data(t)
  local track = t.stream:track('b')
  -- get first event time
  assert_equal(10, track:first())
  -- get next event time
  assert_equal(15, track:next(10))
  -- no next event after 15
  assert_nil(track:next(15))
end

function should.get_track_data_at(t)
  setup_basic_data(t)
  local track = t.stream:track('b')
  -- get data at event time '10' for track 'b'
  assert_equal(ROWS[10].b, track:at(10))
  -- get all data at event time '12'
  assert_equal(ROWS[15].b, track:at(15))
  -- no data at event time '123'
  assert_nil(track:at(12))
end

function should.get_data_range(t)
  setup_basic_data(t)
  local track = t.stream:track('a')
  -- get all data in range [a,b[
  for row in track:range(10,15) do
    if row[1] == 10 then
      assert_equal(ROWS[10].a, row[2])
    elseif row[1] == 12 then
      assert_equal(ROWS[12].a, row[2])
    else
      -- error
      assert_equal(nil, row[1])
    end
  end
  -- last event only
  for row in track:range(15,15) do
    -- should not return any row: range means [a, b[ (b not included)
    assert_true(false)
  end
end

test.all()
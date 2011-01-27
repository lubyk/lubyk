--[[------------------------------------------------------

  db.Stream test
  --------------

  The Stream database is made for recording streaming data
  with timestamps. The data is organized in tracks with
  unique names such as "x", "y", etc.

--]]------------------------------------------------------
require 'rubyk'

local should = test.Suite('db.Stream')

function should.setup(t)
  t.stream = db.Stream('tmp.db')
  -- set data at event time '10'
  t.stream:set{t=10, a=1.1, b=2.0}
  t.stream:set{t=10, c=3}
  t.stream:set{t=12, a=3}
  t.stream:set{t=15, a=5, b=3}
end

function should.teardown(t)
  t.stream:close()
  t.stream = nil
end

function should.open()
  local stream = db.Stream('tmp2.db')
  assert_true(stream:isopen())
  stream:close()
end

function should.get_next_timestamp(t)
  -- get first event time
  assert_equal(10, t.stream:next(0))
  -- get next event time
  assert_equal(12, t.stream:next(10))
  -- no next event after 15
  assert_nil(t.stream:next(15))
end

function should.get_next_track_timestamp(t)
  -- get first event time
  assert_equal(10, t.stream:next(0, 'b'))
  -- get next event time
  assert_equal(15, t.stream:next(10, 'b'))
  -- no next event after 15
  assert_nil(t.stream:next(15, 'b'))
end

function should.get_data_with_at(t)
  -- get all data at event time '10'
  assert_equal({a=1.1, b=2, c=3}, t.stream:at(10))
  -- get all data at event time '15'
  assert_equal({a=5, b=3}, t.stream:at(15))
  -- no data at event time '123'
  assert_nil(t.stream:at(123))
end

function should.get_track_data_with_at(t)
  -- get data at event time '10' for track 'b'
  assert_equal(2, t.stream:at(10, 'b'))
  -- get all data at event time '12'
  assert_equal(3, t.stream:at(15, 'b'))
  -- no data at event time '123'
  assert_nil(t.stream:at(12, 'b'))
end

test.all()
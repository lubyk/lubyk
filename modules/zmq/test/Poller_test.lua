--[[------------------------------------------------------

  zmq.Poller 
  ----------

  The zmq.Poller is used to poll zmq events and normal
  file descriptor events.

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('zmq.Poller')

function should.autoload()
  assertTrue(zmq.Poller)
end

function should.startClean()
  local poller = zmq.Poller()
  local NB = 8
  for i=0,NB-1 do
    assertEqual(nil, poller:posToFd(i))
    assertEqual(-1, poller:idxToPos(i))
    assertEqual(-1, poller:posToIdx(i))
  end
end

function should.add()
  local poller = zmq.Poller()
  local NB = 8
  local idx = poller:add(10, zmq.POLLIN)
  assertEqual(0, idx)
  assertEqual(0, poller:idxToPos(0))
  assertEqual(10, poller:posToFd(0))
  assertEqual(zmq.POLLIN, poller:posToEvent(0))
end

function should.addMany()
  local poller = zmq.Poller()
  local NB = 18
  local idx = {}
  for i=1,NB do
    idx[i] = poller:add(10 + i, zmq.POLLIN)
  end
  for i=1,NB do
    assertEqual(i-1, idx[i])
    assertEqual(i-1, poller:idxToPos(idx[i]))
  end
end

function should.addAndRemove()
  local poller = zmq.Poller()
  local NB = 18
  local idx = {}
  for i=1,NB do
    if i == NB then
      idx[i] = poller:add(897, 1234)
    else
      idx[i] = poller:add(10 + i, zmq.POLLIN)
    end
  end
  assertEqual(897, poller:posToFd(17))
  assertEqual(1234, poller:posToEvent(17))

  -- 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17
  poller:remove(10)
  -- 0 1 2 3 4 5 6 7 8 9 17 11 12 13 14 15 16 nil
  -- idx 10 is removed
  assertEqual(-1, poller:idxToPos(10))
  -- idx 17 moved to 10
  assertEqual(10, poller:idxToPos(17))
  assertEqual(17, poller:posToIdx(10))
  -- removed last
  assertNil(poller:posToEvent(17))

  -- 0 1 2 3 4 5 6 7 8  9 17 11 12 13 14 15 16 nil
  poller:remove(8)
  -- 0 1 2 3 4 5 6 7 16 9 17 11 12 13 14 15 nil nil
  -- idx 8 is removed
  assertEqual(-1, poller:idxToPos(10))
  assertEqual(-1, poller:idxToPos(8))
  -- idx 16 moved to 8
  assertEqual(8, poller:idxToPos(16))
  assertEqual(16, poller:posToIdx(8))
  -- idx 17 moved to 10 (not changed from before)
  assertEqual(10, poller:idxToPos(17))
  assertEqual(17, poller:posToIdx(10))
  assertEqual(897, poller:posToFd(10))
  assertEqual(1234, poller:posToEvent(10))
  -- removed last
  assertNil(poller:posToEvent(16))
  assertNil(poller:posToEvent(17))

  -- 0 1 2 3 4 5 6 7 16 9 17 11 12 13 14 15 nil nil
  local idx = poller:add(80, zmq.POLLIN)
  -- 0 1 2 3 4 5 6 7 16 9 17 11 12 13 14 15 8 nil
  -- idx 8 is used
  assertEqual(16, poller:idxToPos(8))
  assertEqual(8, poller:posToIdx(16))
  --
  assertEqual(-1, poller:idxToPos(10))
  -- idx 16 moved to 8 (not changed from before)
  assertEqual(8, poller:idxToPos(16))
  assertEqual(16, poller:posToIdx(8))
  -- idx 17 moved to 10 (not changed from before)
  assertEqual(10, poller:idxToPos(17))
  assertEqual(17, poller:posToIdx(10))
  assertEqual(80, poller:posToFd(16))
  assertNil(poller:posToEvent(17))
end

function should.change()
  local poller = zmq.Poller()
  local NB = 18
  local idx = {}
  for i=1,NB do
    if i == 10 then
      -- idx = 9
      idx[i] = poller:add(10 + i, 789)
    else
      idx[i] = poller:add(10 + i, zmq.POLLIN)
    end
  end
  assertEqual(789, poller:posToEvent(9))
  assertEqual(20, poller:posToFd(9))
  poller:modify(9, 912, 109)
  for i=1,NB do
    assertEqual(i-1, idx[i])
    if i == 10 then
      assertEqual(912, poller:posToEvent(9))
      assertEqual(109, poller:posToFd(9))
    else
      assertEqual(i-1, poller:idxToPos(idx[i]))
      assertEqual(zmq.POLLIN, poller:posToEvent(idx[i]))
    end
  end
end
test.all()


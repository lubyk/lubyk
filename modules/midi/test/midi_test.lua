--[[------------------------------------------------------

  midi test
  ---------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('midi')

function should.auto_load()
  assert_true(midi)
end

function should.raise_error_on_bad_port()
  local mi = midi.In(88)
  assert_error_match("the 'portNumber' argument (88) is invalid", midi.In(88))
  --assert_true(mi)
  --print(mi:port())
  --io.flush()
  --local continue = false
  --local i= 0
  --function mi.receive(a, b, c)
  --  print(a, b, c)
  --  io.flush()
  --  i = i + 1
  --  if i > 4 then
  --    continue = true
  --  end
  --end
  --while not continue do
  --  sleep(10)
  --end
end

function create_virtual_port()
  local mi = midi.In('foo')
  assert_true(mi)
  print(mi:port())
  io.flush()
  local continue = false
  local i= 0
  function mi.receive(a, b, c)
    print(a, b, c)
    io.flush()
    i = i + 1
    if i > 4 then
      continue = true
    end
  end
  while not continue do
    sleep(10)
  end
end

test.all()

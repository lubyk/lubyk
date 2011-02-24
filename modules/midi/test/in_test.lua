--[[------------------------------------------------------

  midi.In test
  ------------

  ...

--]]------------------------------------------------------
require 'lubyk'

local should = test.Suite('midi.In')

function should.auto_load()
  assert_true(midi.In)
end

function should.raise_error_on_bad_port()
  assert_error("the 'portNumber' argument .88. is invalid", function()
    midi.In(88)
  end)
end

function should.open_port()
  local mi = midi.In(0)
  assert_true(mi)
  print('Found', mi:port_name(), 'please produce midi events...')
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
  local start = worker:now()
  while not continue and worker:now() < start + 2000 do
    sleep(10)
  end
  assert_true(continue)
end

function should.create_virtual_port()
  local mi = midi.In('foo')
  assert_true(mi)
  print('Created virtual port', mi:port_name(), 'please produce midi events...')
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
  local start = worker:now()
  while not continue and worker:now() < start + 2000 do
    sleep(10)
  end
  assert_true(continue)
end

test.all()

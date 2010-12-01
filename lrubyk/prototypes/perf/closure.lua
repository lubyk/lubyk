-- define a mixer processor

-- to avoid empty loop optimization
g_sum = 0

Outlet = {}
function Outlet.new()
  -- private
  local connections = {}

  -- public
  local outlet = {}
  function outlet.send(val)
    for i, con in ipairs(connections) do
      con(val)
    end
  end

  function outlet.connect(remote)
    connections[1] = remote
  end

  return outlet
end

Mixer = {}

function Mixer.new(name)
  -- private
  local val1 = 0
  local val2 = 0
  local sum = Outlet.new()

  local update_sum = function()
    g_sum = (g_sum + val1 + val2) % 100
    sum.send(val1 + val2)
  end

  -- public
  local mixer = {sum = sum}


  function mixer.input1(val)
    --print(name .. '.input1(', val, ')')
    val1 = val
    update_sum()
  end

  function mixer.input2(val)
    --print(name .. '.input2(', val, ')')
    val2 = val
    update_sum()
  end

  return mixer
end

mix1 = Mixer.new('mix1')
mix1.input2(1)
mix1.input1(3)

mix2 = Mixer.new('mix2')
mix2.input2(2)
mix2.input1(4)

print('--------')
mix1.input2(1)
mix1.input1(3)

print('connect mix1 --> mix2')

mix1.sum.connect(mix2.input1)

print('mix1.input1(5)')

mix1.input1(5)

--[[ memory test
data = {}
for i=1,100000 do
  data[i] = Mixer.new('foo')
end

io.read()

-- 95.6 Mb
-- jit 62.6 Mb
--]]

---[[ speed test
for i=1,10000000 do
  mix1.input1(i)
end
print(g_sum)
-- real 0.904 s
-- jit  0.011 s
--]]

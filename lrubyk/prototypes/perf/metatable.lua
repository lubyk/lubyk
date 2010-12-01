-- define a mixer processor

-- to avoid empty loop optimization
g_sum = 0

if not Outlet_mt then
  Outlet_mt = {}
  Outlet_mt.__index = Outlet_mt
end

local mt = Outlet_mt

function Outlet()
  local outlet = {connections = {}}
  setmetatable(outlet, mt)
  return outlet 
end

function mt:send(val)
  for i, con in ipairs(self.connections) do
    -- con[2] = method
    -- con[1] = obj
    con[2](con[1], val)
  end
end

function mt:connect(remote, method)
  self.connections[1] = {remote, method}
end

if not Mixer_mt then
  Mixer_mt = {}
  Mixer_mt.__index = Mixer_mt
end

local mt = Mixer_mt

function Mixer(name)
  local mixer = {name = name, val1 = 0, val2 = 0, sum = Outlet()}
  setmetatable(mixer, mt)
  return mixer
end

function mt:update_sum()
  g_sum = (g_sum + self.val1 + self.val2) % 100
  self.sum:send(self.val1 + self.val2)
end

function mt:input1(val)
  --print(self.name .. '.input1(', val, ')')
  self.val1 = val
  self:update_sum()
end

function mt:input2(val)
  --print(self.name .. '.input2(', val, ')')
  self.val2 = val
  self:update_sum()
end

mix1 = Mixer('mix1')
mix1:input2(1)
mix1:input1(3)

mix2 = Mixer('mix2')
mix2:input2(2)
mix2:input1(4)

print('--------')
mix1:input2(1)
mix1:input1(3)

print('connect mix1 --> mix2')

mix1.sum:connect(mix2, mix2.input1)

print('mix1:input1(5)')

mix1:input1(5)

--[[ memory test
data = {}
for i=1,100000 do
  data[i] = Mixer('foo')
end

io.read()

-- 40.4 Mb
-- jit 29.8 Mb
-- (closure outlet) 58.4 Mb
--]]

---[[ speed test
for i=1,10000000 do
  mix1:input1(i)
end
print(g_sum)
-- real 1.319 s
-- jit real 0.341 s
-- (closure outlet) real 1.276
--]]

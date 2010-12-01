-- we are in the global env
-- rubyk namespace
rk = {loaded = {}}
nodes = {}

-- build a new object from a lua file
function rk.build(filename)
  local code = rk.loaded[filename]
  if not code then
    code = assert(loadfile(filename))
    rk.loaded[filename] = code
  end
  -- new env on each chunk execution (we will optimize to reuse env later)
  local env = {}
  -- inherit global env
  setmetatable(env, {__index = _G})

  -- set function environment to 'env'
  setfenv(code, env)
  -- run code
  code()
  -- return 'node' object
  return env.node
end

-- create and initialize an object if it does not exist or change its settings
function rk.set(name, definition)
  local node = nodes[name]
  if not node then
    local class = definition.class
    node = rk.build(class)
    nodes[name] = node
  end
  node:set(definition)
end

-- execute the first settings and make sure both nodes have their own state
dofile('set1.lua')
a = nodes['a']
b = nodes['b']
assert(a.name == 'A')
assert(b.name == 'B')

a:set{ name = 'Armand' }
assert(a.name == 'Armand')
assert(b.name == 'B')

-- execute the second settings (should not create new objects)
dofile('set2.lua')
assert(nodes['a'] == a)
assert(nodes['b'] == b)

-- should change name
assert(a.name == 'Alpha')
assert(b.name == 'Beta')

print('all ok')
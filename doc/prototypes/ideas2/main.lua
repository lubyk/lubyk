require 'lubyk'

print(worker)

-- we are in the global env
-- lubyk namespace
rk = {loaded = {}}
nodes = {}

-- metatable for the new global env in each node
local mt = {}
function mt.__index(env, name)
  -- if the variable exists in original _G, cache locally
  local gvar = _G[name]
  rawset(env, name, gvar)
  return gvar
end

lk = rk

-- Create a new node environment from a given file
function lk.Environment(filepath)
  local code = assert(loadfile(filepath))
  -- new environment
  local env  = {}
  -- install our global env inheritance system
  setmetatable(env, mt)
  -- set function environment to 'env'
  setfenv(code, env)
  -- run code
  code()
  -- return global 'node' object from the new environment
  return env.node
end

-- create and initialize an object if it does not exist or change its settings
function lk.set(name, definition)
  local node = nodes[name]
  if not node then
    local class = definition.class
    node = rk.Environment(class)
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
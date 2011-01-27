-- we are in the global env
dofile('Node.lua')
-- lubyk namespace
rk = {prototype = {}}
nodes = {}

-- load/reload a filename. If env is provided it will be use
-- as _G.
function rk.load(filename, env)
  local code = assert(loadfile(filename))
  local env = env or {node = Node.new()}
  -- inherit global env
  setmetatable(env, {__index = _G})
  -- set function environment to 'env'
  setfenv(code, env)

  -- enable sub-patch building
  local nodes_bak = nodes
    nodes = env.node
    -- run code
    code()
  -- restore nodes
  nodes = nodes_bak
  -- use 'node' object as prototype
  local prototype = env.node
  prototype._env = env -- keep track of environment in case we reload
  -- when reloading, this should do nothing (same object)
  rk.prototype[filename] = prototype
  return prototype
end

-- return prototype from a given filename
function rk.class(filename)
  local prototype = rk.prototype[filename]
  if not prototype then
    return rk.load(filename)
  end
  return prototype
end

-- reload a filename (re-eval script)
function rk.reload(filename)
  local prototype = rk.prototype[filename]
  if prototype then
    -- reload
    return rk.load(filename, prototype._env)
  else
    -- load
    return rk.class(filename)
  end
end

-- build a new object from a lua file
function rk.build(filename)
  local prototype = rk.class(filename)
  -- create new instance of the prototype
  local node = {prototype = prototype}
  node.outlets = OutletList.new(node, prototype._outlets)
  setmetatable(node, {__index = prototype})
  return node
end

-- create and initialize an object if it does not exist or change its settings
function rk.set(name, definition)
  local node = nodes[name]
  if not node then
    local class = definition.class
    node = rk.build(class)
    nodes[name] = node
  end
  if node.set then
    node:set(definition)
  end
end

function rk.connect(obj1, outlet, obj2, inlet)
  local target = nodes[obj2]
  nodes[obj1].outlets[outlet]:connect(target,target.inlets[inlet].call)
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

-- script reload should update prototype (not break existing link)
rk.reload('proto.lua')

-- if we change the prototype (user changes script: should update both objects)
local proto = rk.class('proto.lua')

function proto:hello()
  return string.format('Hello from %s', self.name)
end

-- should change name
assert(a:hello() == 'Hello from Alpha')
assert(b:hello() == 'Hello from Beta')


-- we should be able to use the accessors
assert(a.inlets.input.call(a, 'hello') == 'hello')
-- value should be sent to b
assert(b.val == 'Alpha: hello')
assert(b.inlets.input.call(b, 'goodbye') == 'goodbye')
-- b should not send to a
assert(a.val == 'hello')

assert(a.inlets.input.call(a) == 'hello')
assert(a.val == 'hello')
a.input2 = 'foo'
assert(a.inlets.input2.call(a) == 'foo')
assert(a.inlets.input2.call(a, 'baz') == 'baz')
assert(a.input2 == 'baz')

print('all ok')
--[[------------------------------------------------------

  rk.Properties
  -------------

  Storage with notification on change.

--]]------------------------------------------------------
require 'rubyk'

x = rk.Properties()

local Boo = {}
Boo.__index = Boo

local node = {name = 'node'}
setmetatable(node, Boo)
local other = {name = 'other'}
setmetatable(other, Boo)

-- inlet callback
function Boo:foo_changed(new_value)
  print('Boo', self.name, new_value)
end

function other:foo_changed(new_value)
  print('barbar', self.name, new_value)
end

x:connect('foo', node, 'foo_changed')
x:connect('foo', node, 'foo_changed')
x:connect('foo', other, 'foo_changed')


x.foo = 5
x.foo = 6
x.foo = 6
print(x.foo)



-- inlet callback
function Boo:foo_changed(new_value)
  print('Boo2', self.name, new_value)
end

function other:foo_changed(new_value)
  print('barbar2', self.name, new_value)
end

x:connect('foo', node, 'foo_changed')
x:connect('foo', node, 'foo_changed')
x:connect('foo', other, 'foo_changed')


x.foo = 5
x.foo = 6
x.foo = 6
print(x.foo)

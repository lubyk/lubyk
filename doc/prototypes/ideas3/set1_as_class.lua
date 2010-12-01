dofile('main.lua')

-- this should create s.a, s.b
rk.set('s', {class='set1.lua', name='A'})

s = nodes.s
assert(s.a.name == 'A')
assert(s.b.name == 'B')
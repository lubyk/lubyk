rk.set('a', {class='proto.lua', name='A'})
rk.set('b', {class='proto.lua', name='B'})
-- links
rk.connect('a', 'output', 'b', 'input')
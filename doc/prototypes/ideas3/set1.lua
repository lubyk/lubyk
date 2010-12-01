-- when we save a setting into a library: how do we create inlets/outlets ?
rk.set('a', {class='proto.lua', name='A'})
rk.set('b', {class='proto.lua', name='B'})
-- links
nodes.a.outlets.output:connect(nodes.b,nodes.b.inlets.input.call)


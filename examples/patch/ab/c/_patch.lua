require 'lubyk'

app = mimas.Application()

patch = lk.Process [[
x: 100
y: 100
w: 150
h: 150
hue: 0,5
nodes:
  loop:
    x: 230
    y: 45
    hue: 0,5
]]

app:exec()
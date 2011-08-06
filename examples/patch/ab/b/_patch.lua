require 'lubyk'

app = mimas.Application()

patch = lk.Process [[
x: 110
y: 200
w: 150
h: 150
hue: 0.3
nodes:
  play:
    x: 70
    y: 95
    hue: 0.3
    params:
      channel: 2
]]

app:exec()

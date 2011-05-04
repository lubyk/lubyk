require 'lubyk'

app = mimas.Application()

patch = lk.Process [[
x: 300
y: 150
w: 150
h: 150
hue: 0,2
]]

app:exec()

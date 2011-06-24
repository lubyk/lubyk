require 'lubyk'

app = mimas.Application()

patch = lk.Process [[
x: 100
y: 100
w: 150
h: 150
hue: 0
nodes:
  metro:
    class: lubyk.Metro
    x: 30
    y: 45
    hue: 0
    links:
      bang:
        counter/in/bang: true
    params:
      tempo: 580
  counter:
    x: 30
    y: 45
    hue: 0
    links:
      count:
        /b/play/in/trigger: true
    params:
      tempo: 580
]]

app:exec()

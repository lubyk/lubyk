require 'lubyk'

app = mimas.Application()

patch = lk.Process [[
w: 150
h: 150
nodes:
  metro:
    class: lubyk.Metro
    x: 30
    y: 45
    hue: 0
    links:
      bang:
        play/in/trigger: true
    params:
      tempo: 580

  play:
    x: 70
    y: 95
    hue: 0,3
    params:
      channel: 2
]]

app:exec()

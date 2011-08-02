require 'lubyk'

app = mimas.Application()
patch = lk.Process [[
nodes:
  beat:
    class: lubyk.Metro
    x: 30
    y: 15
    links:
      bang: grid/in/advance
    params:
      tempo: 240

  grid:
    x: 30
    y: 55
    links:
      hit: play/in/hit
      updated: window/in/update

  play:
    x: 70
    y: 135

  window:
    x: 70
    y: 135
    links:
      paint:  grid/in/paint
]]

app:exec()

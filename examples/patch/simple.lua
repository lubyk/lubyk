require 'lubyk'

patch = lk.Patch [[
beat:
  class: lubyk.Metro
  x: 30
  y: 15
  links:
    bang: beep/in/trigger
  params:
    tempo: 580

beep:
  script: |
    inlet('trigger', 'Trigger midi note [bang].')
    mo = mo or midi.Out('lubyk')
    channel = channel or 1
    function inlet.trigger() -- noteOn
      print('bang!', channel)
      local n = 48 + 12 * math.random()
      -- FIXME better midi.Out with note length...
      mo:send(143 + channel, n, 70)
      sleep(100)
      mo:send(127 + channel, n, 70) -- noteOff
    end
  x: 70
  y: 95
  params:
    channel: 2
]]

sleep(4000)
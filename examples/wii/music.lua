require 'lubyk'

patch = lk.Process [[
w: 250
h: 150
nodes:
  wii:
    class: lubyk.Wii
    x: 130
    y: 45
    hue: 0.4

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
    code: |
      ---- Receives acceleration and btn information from wii and plays sounds
      inlet('trigger', 'Trigger midi note [bang].')
      inlet('note', 'Midi note value [float].')
      inlet('onOff', 'Turn sound on or off [btn,state].')
      
      mo = mo or midi.Out('lubyk')
      channel = channel or 1
      
      note = 45
      play = true
      velo = 50
      function inlet.note(n, y)
        note = 45 + n * 36
        velo = 10 + math.max(y, 0) * 50
        print(y)
      end
      
      function inlet.trigger() -- noteOn
        if play then
          local n = note
          mo:send(143 + 1, n, velo)
          sleep(20)
          mo:send(127 + 2, n, velo) -- noteOff
        end
      end
      
      function inlet.onOff(btn, state)
        -- pres == off
        play = not state
      end
    x: 70
    y: 95
    hue: 0.3
    params:
      channel: 2
]]

--sleep(40000)
worker:run()
-- FIXME: when patch is deleted, all nodes do not seem to die...


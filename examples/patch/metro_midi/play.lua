--[[------------------------------------------------------

  play
  ----

  When the object receives a bang, send a midi event out.

--]]------------------------------------------------------

inlet('trigger', 'Trigger midi note [bang].')
mo = mo or midi.Out('lubyk')
channel = channel or 1
count = count or 1
notes  = {40, 43, 47, 48, 50, 52, 48, 50}
notes  = {40, 43, 47, 48} --, 50, 52, 48, 52}
notes2 = {48, 0, 54, 0, 48, 0, 54, 52}

function inlet.trigger() -- noteOn
  count = count + 1
  local n2 = 48 + 12 * math.random()
  -- FIXME better midi.Out with note length...
  local i = math.floor(math.random() * 4) + 1
  local n  = notes[i] + 12
  --local n  = notes[(count % #notes) + 1] + 12
  local n2 = notes2[(i % #notes2) + 1] - 12
  local n2 = notes2[(count % #notes2) + 1] - 12
  if n then
    mo:send(143 + 1, n, 50)
    mo:send(143 + 2, n2, 70)
    sleep(20)
    mo:send(127 + 2, n2, 70) -- noteOff
    mo:send(127 + 1, n, 70) -- noteOff
  end
end


print('Reloaded')
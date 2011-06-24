--[[------------------------------------------------------

  play
  ----

  When the object receives a bang, send a midi event out.

--]]------------------------------------------------------

inlet('trigger', 'Trigger midi note [bang].')
mo = mo or midi.Out('lubyk')
channel = channel or 1
count = count or 1
list  = {
{
'C4', 'D4', 'Eb4', 'Bb4', 'F4', 'C5', 'G4', 'Ab4',
'F4', 'F1' , 'G4', 'F1' ,'F4', 'F1' , 'B4', 'F1' ,
'F4', 'F1' , 'Ab4', 'F1' ,'F4', 'F1' , 'G4', 'F1' ,
--},
--{
'C4', 'D4', 'Eb4', 'Bb4', 'G4', 'F4', 'C5', 'Ab4',
'F4', 'F1' , 'Ab4', 'F1' ,'G4', 'F1' , 'C5', 'F1' ,
'F4', 'F1' , 'B4', 'F1' ,'G4', 'F1' , 'Ab4', 'F1' ,
},
{
'C2', ''  ,  'G3', 'Ab3',
'C2', 'Eb3', 'G3', 'D3',

'G3', ''  , 'F4', 'C5',
'G3', 'Eb3', 'F4', 'C5',
}
}

notes  = list[1]

-- drums
notes2 = {
48, 54, 50, 54, 48, 52, 50, 54,
}

function inlet.trigger() -- noteOn
  count = count + 1
  local n2 = 48 + 12 * math.random()
  -- FIXME better midi.Out with note length...
  local i = math.floor(math.random() * #notes) + 1
  --local n  = notes[i] + 12
  local n = notes[(count % #notes) + 1]
  if n == 'F1' then
    n = 'F1'
  else
    n = nil
  end
  n  = midi.Note[n]

  n = n

  local n2 = notes2[(i % #notes2) + 1] - 12
  local n2 = notes2[(count % #notes2) + 1] - 12
  if n and n > 0 then
    n = n
    --mo:send(143 + 1, n, 50)
  end
  if n2 then
    --mo:send(143 + 2, n2, 70)
  end
  wait(10)
  if n2 then
    mo:send(127 + 2, n2, 70) -- noteOff
  end
  if n then
    mo:send(127 + 1, n, 70) -- noteOff
  end
end


print('Reloaded')

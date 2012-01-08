--[[------------------------------------------------------

  Metro
  -----

  Triggers regular bangs out of it's unique outlet.

--]]------------------------------------------------------
inlet('tempo', 'Set tempo [float: bpm].')
outlet('bang', 'Sends a bang on every beat [true].')

-- Public values
tempo = tempo or 120

timer = nil
-- Private
if not timer then
  timer = lk.Timer(tempo, function()
    bang(true)
  end)
  timer:start()
end

-- Accessor
function inlet.tempo(val)
  tempo = val
  if tempo > 0 then
    -- interval in [ms]
    timer:setInterval(60000 / tempo)
  else
    timer:stop()
  end
end

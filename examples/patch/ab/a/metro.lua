--[[------------------------------------------------------

  Metro
  -----

  Triggers regular bangs out of it's unique outlet.

--]]------------------------------------------------------
inlet('tempo', 'Set tempo [float: bpm].')
outlet('bang', 'Sends a bang on every beat [true].')

-- Public parameters.
defaults {
  tempo = 120,
}

-- Private
if not timer then
  if tempo > 0 then
    timer = lk.Timer(60000 / tempo)
    timer:start()
  else
    timer = lk.Timer(0)
  end
end

-- Accessor
function inlet.tempo(val)
  tempo = val
  if tempo > 0 then
    -- interval in [ms]
    timer:setInterval(60000 / tempo, true)
  else
    timer:stop()
  end
end

function timer:tick()
  bang(true)
end

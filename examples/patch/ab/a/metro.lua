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
  timer = lk.Thread(function()
    while timer:shouldRun() do
      timer:run()
    end
  end)
end

-- Accessor
function inlet.tempo(va)
  tempo = val
  if tempo > 0 then
    -- interval in [ms]
    timer:setInterval(60000 / tempo)
  else
    timer:stop()
  end
end

function timer:run()
  sleep(10)
  --sleep(0.01)
  bang(true)
end

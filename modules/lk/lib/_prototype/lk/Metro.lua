--[[------------------------------------------------------

  h1. lk.Metro

  Triggers regular bangs out of it's unique outlet.

  Inlets
  --------------------------------------------------------
  [tempo]    Set the tempo in beats per minute.
  [freq]     Set the frequency in Hz.


  Outlets
  --------------------------------------------------------
  [bang]     Sends 'true'.

--]]
local i, o, p, print = lubyk()

o.bang = {}

p {
  tempo = 120, -- in beats per minute
  freq  = 2,   -- in Hz
}

step = 0.5

if not timer then
  if tempo > 0 then
    timer = lk.Timer(60000 / tempo)
    timer:start()
  else
    timer = lk.Timer(0)
  end
end

--=============================================== params

function p.tempo(val)
  setFreq(val / 60)
  p.freq = val / 60
end

function p.freq(val)
  setFreq(val)
  p.tempo = val * 60
end

--=============================================== inlets
function i.tempo(val)
  setFreq(val)
  -- notify change.
  p.freq  = val
  p.tempo = val * 60
end

--=============================================== code
function setFreq(val)
  if val > 0 then
    -- interval in [ms]
    step = 1/val
    timer:setInterval(1000 * step)
    if timer.stopped then
      timer.stopped = nil
      timer:start()
    end
  else
    timer.stopped = true
    timer:stop()
  end
end

local bang = o.bang
function timer:tick()
  bang(step)
end

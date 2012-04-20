--[[------------------------------------------------------

  lk.Metro
  --------

  Triggers regular bangs out of it's unique outlet.

  Inlets
  --------------------------------------------------------
  [tempo]    Set the tempo in beats per minute.
  [freq]     Set the frequency in Hz.


  Outlets
  --------------------------------------------------------
  [ban]      Sends 'true'.

--]]
outlet 'bang'

defaults {
  tempo = 120,
  freq  = 2,
}

if not timer then
  if tempo > 0 then
    timer = lk.Timer(60000 / tempo)
    timer:start()
  else
    timer = lk.Timer(0)
  end
end

--=============================================== params

function param.tempo(val)
  setFreq(val / 60)
  param.freq = val / 60
end

function param.freq(val)
  setFreq(val)
  param.tempo = val * 60
end

--=============================================== inlets
function inlet.tempo(val)
  setFreq(val)
  -- notify change.
  param.freq  = val
  param.tempo = val * 60
end

--=============================================== code
function setFreq(val)
  if val > 0 then
    -- interval in [ms]
    timer:setInterval(1000 / val)
  else
    timer:stop()
  end
end

function timer:tick()
  bang(true)
end

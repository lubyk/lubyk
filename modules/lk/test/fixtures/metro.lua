inlet('tempo', 'Receive tempo.')
bang  = outlet('bang', 'Sends bangs on every beat [true].')
tempo = tempo or 0

if not timer then
  timer = lk.Timer(tempo, function()
    bang(true)
  end)
  if tempo > 0 then
    timer:start()
  end
end

function inlet.tempo(val)
  if val > 0 then
    timer:set_interval(60000/tempo)
    if tempo == 0 then
      -- will bang on start
      timer:start()
    end
  else
    tempo = 0
    timer:stop()
  end
end

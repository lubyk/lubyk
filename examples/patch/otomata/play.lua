--[[------------------------------------------------------

  play
  ----

  When the objects hit walls, we play sounds.

--]]------------------------------------------------------
inlet('hit', 'Hit position [int, int].')

mo = mo or midi.Out('automata')

local NOTES = {35, 37, 40, 42, 44, 47, 49, 52, 54, 56}

function inlet.hit(row, col)
  n = NOTES[row] or NOTES[col]
  if n then
    mo:send(143 + 1, n, 50)
    -- FIXME: implement note length in midi.Out !!!
    sleep(20)
    mo:send(127 + 1, n, 70) -- noteOff
  end
end

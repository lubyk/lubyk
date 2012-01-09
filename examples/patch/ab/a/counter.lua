--[[------------------------------------------------------

  counter
  -------

  Send increasing numbers on each bang.

--]]------------------------------------------------------

inlet('bang', 'Send next number on [bang].')
count = outlet('count', 'Increasing numbers [float]')

defaults {
  -- 50 ms
  step = 50,
}

t = t or 0

function inlet.bang(...)
  t = t + step
  count(t)
end

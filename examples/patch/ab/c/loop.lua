--[[------------------------------------------------------

  counter
  -------

  Send increasing numbers on each bang.

--]]------------------------------------------------------

inlet('bang', 'Send next number on [bang].')
pass = outlet('pass', 'Increasing numbers [float]')

function inlet.bang(i)
  i = i + 1
  pass(i)
end

--[[------------------------------------------------------

  counter
  -------

  Send increasing numbers on each bang.

--]]------------------------------------------------------

inlet('bang', 'Send next number on [bang].')
count = outlet('count', 'Increasing numbers [float]')

i = i or 0

function inlet.bang(...)
  i = i + 1
  count(i, 'hello')
end

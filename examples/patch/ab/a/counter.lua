--[[------------------------------------------------------

  counter
  -------

  Send increasing numbers on each bang.

--]]------------------------------------------------------

inlet('bang', 'Send next number on [bang].')
count = outlet('count', 'Increasing numbers [float]')

i = i or 0
local large_msg = '1234567890'
large_msg = large_msg .. large_msg
large_msg = large_msg .. large_msg
large_msg = large_msg .. large_msg
large_msg = large_msg .. large_msg
large_msg = large_msg .. large_msg
large_msg = large_msg .. large_msg
large_msg = large_msg .. large_msg
large_msg = large_msg .. large_msg
local medium_msg = large_msg
large_msg = large_msg .. large_msg
large_msg = large_msg .. large_msg

function inlet.bang(...)
  i = i + 1
  count(i, large_msg)
end
print("++++++++++++++++++++++++++++")
print('LARGE  MESSAGE (Ko):', string.len(large_msg)/1024)
print('MEDIUM MESSAGE (Ko):', string.len(medium_msg)/1024)

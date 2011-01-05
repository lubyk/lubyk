--[[------------------------------------------------------

  worker
  ------

  The worker is an essential part as it is passed to all
  Cpp code that needs to callback (the worker holds a
  mutex).

--]]------------------------------------------------------
require 'rubyk.Worker'
worker = rubyk.Worker()
function rubyk.Worker()
  error('Already one worker created (lives in _worker).')
end

function sleep(duration)
  worker:sleep(duration)
end
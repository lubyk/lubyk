--[[------------------------------------------------------

  worker
  ------

  The worker is an essential part as it is passed to all
  Cpp code that needs to callback (the worker holds a
  mutex).

--]]------------------------------------------------------
require 'lubyk.Worker'
worker = lubyk.Worker()
function lubyk.Worker()
  error('Already one worker created (lives as \'worker\' in global namespace).')
end

function sleep(duration)
  worker:sleep(duration)
end

function wait(duration)
  worker:wait(duration)
end
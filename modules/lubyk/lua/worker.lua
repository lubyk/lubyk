--[[------------------------------------------------------

  worker
  ------

  The worker is an essential part as it is passed to all
  Cpp code that needs to callback (the worker holds a
  mutex).

--]]------------------------------------------------------
require 'lubyk.Worker'
worker   = lubyk.Worker()
local mt = lubyk.Worker_

function lubyk.Worker()
  error('Already one worker created (lives as \'worker\' in global namespace).')
end

function sleep(duration)
  worker:sleep(duration)
end

function wait(duration)
  worker:wait(duration)
end

local spawn = lubyk.Worker_.spawn
function mt:spawn(code, args)
  local dump = yaml.dump(args)
  local sep = ''
  -- Make sure the literal string in the yaml dump is not finished too soon by the
  -- text in the dump.
  while string.match(dump, string.format('\]%s\]', sep)) do
    sep = sep .. '='
  end

  local args = string.format("yaml.load [%s[\n%s\n]%s]", sep, dump, sep)
  -- spawn
  return spawn(self, string.format(code, args))
end

--[[------------------------------------------------------

  worker
  ------

  This worker is only needed for accurate sleep and other
  OS specific things like spawning process. This could be
  renamed as lk.System...

--]]------------------------------------------------------
require 'lubyk.Worker'
worker   = lubyk.Worker()
local mt = lubyk.Worker_

function lubyk.Worker()
  error('Already one worker created (lives as \'worker\' in global namespace).')
end

local spawn = lubyk.Worker_.spawn
--- We use yaml to pass arguments from one process to the other.
function mt:spawn(code, args)
  local dump = yaml.dump(args)
  local sep = ''
  -- Make sure the literal string in the yaml dump is not finished too soon by the
  -- text in the dump.
  while dump:match(string.format('%%]%s%%]', sep)) do
    sep = sep .. '='
  end

  local args = string.format("yaml.load [%s[\n%s\n]%s]", sep, dump, sep)
  return spawn(self, string.format(code, args))
end

function mt:execute(code)
  return io.popen(code)
end

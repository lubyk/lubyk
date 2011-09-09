-- This should work without needing lubyk to be installed
local code = loadfile(string.format('%s/.lubyk', os.getenv('HOME')))
if code then
  Lubyk = code()
else
  Lubyk = {}
end

Lubyk.lib = arg[1]
if Lubyk.lib then
  package.path  = Lubyk.lib .. '/?.lua;'
  package.cpath = Lubyk.lib .. '/?.so;'
  require 'lk'
else
  require 'lubyk'
end
app = editor.Application()
app:exec()

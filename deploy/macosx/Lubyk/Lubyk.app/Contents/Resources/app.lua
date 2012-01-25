-- This should work without needing lubyk to be installed
if arg[1] then
  local code = loadfile(string.format('%s/.lubyk', os.getenv('HOME')))
  if code then
    Lubyk = code()
  else
    Lubyk = {}
  end
  Lubyk.lib = arg[1]
  package.path  = Lubyk.lib .. '/?.lua;' .. package.path
  package.cpath = Lubyk.lib .. '/?.so;'  .. package.cpath
  require 'lk'
else
  require 'lubyk'
end
app = editor.Application()
run()

-- This should work without needing lubyk to be installed
Lubyk = {lib = arg[1]}
if Lubyk.lib then
  package.path  = Lubyk.lib .. '/?.lua;'
  package.cpath = Lubyk.lib .. '/?.so;'
  require 'lk'
else
  require 'lubyk'
end
app = editor.Application()
app:exec()

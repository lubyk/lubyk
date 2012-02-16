-- This should work without needing lubyk to be installed
if arg[1] then
  package.path  = arg[1] .. '/?.lua;' .. package.path
  package.cpath = arg[1] .. '/?.so;'  .. package.cpath
end
require 'lubyk'
app = editor.Application(arg)
run()

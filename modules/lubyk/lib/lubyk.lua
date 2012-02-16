--[[------------------------------------------------------

  lubyk (mac)
  -----------

  The role of this file is to alter the load paths so that
  lua can find Lubyk libraries. This file also loads user
  settings from ~/.lubyk to let these settings overwrite
  the default library path.

--]]------------------------------------------------------
local code = loadfile(string.format('%s/.lubyk/lk.lua', os.getenv('HOME')))
if code then
  Lubyk = code()
else
  Lubyk = {}
end

--- PLATFORM SPECIFIC
Lubyk.lib  = Lubyk.lib  or '/Applications/Lubyk/lib'
Lubyk.plat = Lubyk.plat or 'macosx'
--- END PLATFORM SPECIFIC
package.path  = Lubyk.lib .. '/?.lua;' .. package.path
package.cpath = Lubyk.lib .. '/?.so;'  .. package.cpath
require 'lk'


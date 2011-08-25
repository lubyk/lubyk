--[[------------------------------------------------------

  Lubyk
  -----

  The role of this file is to alter the load paths and
  prepare for lubyk lib loading. This file also loads user
  settings from ~/.lubyk

--]]------------------------------------------------------
local code = loadfile(string.format('%s/.lubykrc', os.getenv('HOME')))
_lubyk_settings = {paths = {}, cpaths = {}, editor = {}}
if code then
  _lubyk_settings = code()
  _lubyk_settings.editor = _lubyk_settings.editor or {}
end
local ls = _lubyk_settings
-- search order:
local lua_paths  = package.path
local lua_cpaths = package.cpath
-- 1. local lib (foo.lua or lib/foo.lua)
package.path  = './?.lua;./lib/?.lua;'
package.cpath = './?.so;./lib/?.so;'
-- 2. user libs
for _, path in ipairs(_lubyk_settings.paths) do
  package.path = package.path .. path .. ';'
end
for _, cpath in ipairs(_lubyk_settings.cpaths) do
  package.cpath = package.cpath .. cpath .. ';'
end
ls.lubyk_lib_path = ls.lubyk_lib_path or '/usr/local/lib/lubyk'
-- 3. Installed lubyk libs
package.path  = package.path  .. ls.lubyk_lib_path .. '/?.lua;'
package.cpath = package.cpath .. ls.lubyk_lib_path .. '/?.so;'
-- 4. Installed lua paths
package.path  = package.path  .. lua_paths
package.cpath = package.cpath .. lua_cpaths

require 'lk'
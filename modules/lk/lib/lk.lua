--[[------------------------------------------------------

  lk
  --

  This file is loaded just after the load paths have been
  changed and loads Lubyk essentials.

--]]------------------------------------------------------

--=============================================== LOAD PATHS

-- These paths already come loaded with lubyk lib path
-- through the lubyk loader "require 'lubyk'"
local lua_paths  = package.path
local lua_cpaths = package.cpath

-- 1. local lib (foo.lua or lib/foo.lua)
package.path  = './?.lua;./lib/?.lua;'
package.cpath = './?.so;./lib/?.so;'

-- 2. user libs
for _, path in ipairs(Lubyk.paths or {}) do
  package.path = package.path .. path .. '/?.lua;'
end

for _, cpath in ipairs(Lubyk.cpaths or {}) do
  package.cpath = package.cpath .. cpath .. '/?.so;'
end

-- 3. Installed lubyk libs and lua paths
package.path  = package.path  .. lua_paths
package.cpath = package.cpath .. lua_cpaths

--=============================================== BASE REQUIRE
-- From core module
require 'lk.core'
-- From lk module
require 'lk.more'

--[[------------------------------------------------------

  Rubyk
  -----

  The role of this file is to alter the load paths and
  prepare for rubyk lib loading. It is usually the only
  file stored in the usual Lua load path.

--]]------------------------------------------------------

-- local/share/lua/5.1/?.lua;/usr/local/share/lua/5.1/?/init.lua;/usr/local/lib/lua/5.1/?.lua;/usr/local/lib/lua/5.1/?/init.lua
package.path = "./?.lua;./lib/?.lua;/usr/local/lib/rubyk/?.lua;/Users/gaspard/git/rubyk/lib/?.lua"
require 'rk'

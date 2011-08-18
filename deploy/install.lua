#!/usr/bin/env lua
local function doCmd(cmd)
  print(cmd)
  os.execute(cmd)
end

package.path = './?.lua;./lib/?.lua;../lib/?.lua;' .. package.path
package.cpath = './?.so;./lib/?.so;../lib/?.so;' .. package.cpath
require 'lubyk'
local lib_path = lk.absolutizePath(lk.directory(lk.file())..'/../lib')
local lubykrc = string.format([[
local settings = {}
-- additional paths
local base = [=[%s]=]
settings.paths = {
  -- Load manual build content first
  base .. '/?.lua',
}

settings.cpaths = {
  -- Load manual build content first
  base .. '/?.so',
}

settings.editor = {
  editor_cmd = os.getenv('EDITOR') or 'vim',
  library_sources = {
    lubyk = base .. '/lubyk',
  }
}
return settings
]], lib_path)
-- get install dir from package.path
doCmd "sudo cp lubyk_loader.lua /usr/local/lib/lua/5.1/lubyk.lua"
doCmd "chmod a+x lubyk_cmd"
doCmd "sudo cp lubyk_cmd /usr/local/bin/lubyk"
local rcfile = os.getenv('HOME') .. '/.lubykrc'
if not lk.exist(rcfile) then
  local file = assert(io.open(rcfile, 'w'))
  file:write(lubykrc)
  file:close()
end
doCmd"/usr/local/bin/lubyk --testInstall"

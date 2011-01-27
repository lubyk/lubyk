lua_install_path = package.path
package.cpath = 'lib/?.so'
package.path = 'lib/?.lua'
require 'lfs'

-- copy modules/lubyk/lua/lubyk.lua
-- in first folder not local in lua_install_path
print(lfs._VERSION)
--[[------------------------------------------------------

  lk.Lubyk
  --------

  Default settings (sets a metatable to Lubyk)

--]]------------------------------------------------------
local version = {
  major = 0,
  minor = 5,
}
----------------------------------------------------------

local Lubyk   = {}
Lubyk.__index = Lubyk

function lk.Lubyk(tbl)
  return setmetatable(tbl, Lubyk)
end

Lubyk.zone = 'default'
Lubyk.version = version
Lubyk.service_type = '_lubyk._tcp'
Lubyk.prototypes_lib = string.format('%s/Documents/lubyk.sqlite3', os.getenv('HOME'))
Lubyk.library_sources = {}

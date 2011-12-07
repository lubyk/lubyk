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
Lubyk.prototypes_lib = string.format('%s/Documents/Lubyk.db', os.getenv('HOME'))
Lubyk.library_sources = {}
Lubyk.dav_port = 8103 -- BLOK
-- This is set to false by lk.Patch so that lk.Process do not quit on closing windows.
Lubyk.mimas_quit_on_close = true

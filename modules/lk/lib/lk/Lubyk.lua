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
  for k, v in pairs(tbl) do
    if type(v) == 'table' and type(Lubyk[k]) == 'table' then
      Lubyk[k].__index = Lubyk[k]
      setmetatable(v, Lubyk[k])
    end
  end
  return setmetatable(tbl, Lubyk)
end

Lubyk.zone = 'default'
Lubyk.version = version
Lubyk.service_type = '_lubyk._tcp'
Lubyk.prototypes_lib = string.format('%s/Documents/Lubyk.db', os.getenv('HOME'))
Lubyk.editor = {
  base_library_sources = {
    -- Paths relative to Lubyk.lib
    lubyk = 'lubyk',
  },
  -- Absolute paths (user setings)
  library_sources = {},
  -- Which views to show on launch.
  show = {
    Patch = true,
  },
}
Lubyk.dav_port = 8103 -- BLOK
-- This is set to false by lk.Patch so that lk.Process do not quit on closing windows.
Lubyk.mimas_quit_on_close = true

-- Default host name
local function getMachineName()
  local file = io.popen('hostname')
  local name = file:read()
  local short_name = string.match(name, '^([^-.]+)')
  return short_name or name
end
Lubyk.host = getMachineName()


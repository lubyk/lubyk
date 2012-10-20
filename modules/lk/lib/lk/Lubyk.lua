--[[------------------------------------------------------

  lk.Lubyk
  --------

  Default settings (sets a metatable to Lubyk)

--]]------------------------------------------------------
local version = {
  major = 0,
  minor = 5,
  patch = 1,
}
----------------------------------------------------------

local defaults = {
  zone = 'default',
  version = version,
  service_type = '_lubyk._tcp',
  --- WebDAV port for live coding
  dav_port = 8103,
  -- This is set to false by lk.Patch so that lk.Process do not quit on closing windows.
  mimas_quit_on_close = true,
  -- The following settings are set in bootstrap loader lubyk.lua
  -- and are platform specific.
  
  -- Main location for lubyk libraries
  lib = Lubyk and Lubyk.lib,

  -- Platform code (linux,macosx,windows)
  plat= Lubyk and Lubyk.plat,
}

Lubyk = lk.Settings('lk', defaults)
-- Cannot be changed, no need for copy on write.
Lubyk.version = defaults.version
Lubyk.version_str = string.format('%i.%i.%i', version.major, version.minor, version.patch)

-- Default host name
local function getMachineName()
  local file = io.popen('hostname')
  local name = file:read()
  local short_name = string.match(name or '', '^([^-.]+)')
  return short_name or name or 'unknown'
end
defaults.host = getMachineName()


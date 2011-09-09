--[[------------------------------------------------------

  wii.Browser
  -----------

  @internal (used to browse for wiimotes)

--]]------------------------------------------------------
require 'wii'
require 'wii.Browser_core'
require 'wii.Remote_core'
require 'worker'

-- Singleton
wii.Browser = wii.Browser()
--local constr = wii.Browser
--function wii.Browser()
--  local instance = constr(worker)
--  if func then
--    instance.found = func
--  else
--    function instance.found
--  end
--  return instance
--end

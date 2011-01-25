--[[------------------------------------------------------

  wii.Remote
  ----------

  Access to a Wii Remote controller.

--]]------------------------------------------------------
require 'wii'
require 'wii.Remote_core'
require 'worker'

local constr = wii.Remote
local worker = worker
function wii.Remote(name)
  local instance = constr(worker, name)
  return instance
end

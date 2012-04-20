--[[------------------------------------------------------

  mdns.Service
  ------------

  This class is returned from mdns.Browser.

--]]------------------------------------------------------
require 'mdns.core'
local lib = mdns.Service_core
local constr = lib.new
local private = {}

-- Disable constructor from Lua for now.
mdns.Service = nil

--[[------------------------------------------------------

  rk
  -----

  This file is loaded just after the load paths have been
  changed and loads Rubyk essentials.

--]]------------------------------------------------------
require 'Autoload'
rk = Autoload('rk')
-- autoload stuff in _G
Autoload.global()

rubyk = Autoload('rubyk')
-------------------------------- CONSTANTS
rubyk.service_type = '_rubyk._tcp'

-------------------------------- rubyk.core
require 'rubyk.core'

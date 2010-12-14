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

-- loads 'rubyk'
require 'rubyk.core'

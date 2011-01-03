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
rubyk.service_type    = '_rubyk._tcp'
rubyk.sub_port_url    = '/rk/sub_port'
rubyk.add_service_url = '/rk/add_service'
rubyk.rem_service_url = '/rk/rem_service'

-------------------------------- rubyk.core
require 'rubyk.core'

-------------------------------- pack/unpack (msgpack.vendor)
require 'msgpack.vendor'

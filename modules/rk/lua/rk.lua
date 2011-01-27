--[[------------------------------------------------------

  rk
  -----

  This file is loaded just after the load paths have been
  changed and loads Lubyk essentials.

--]]------------------------------------------------------
require 'Autoload'
rk = Autoload('rk')
-- autoload stuff in _G
Autoload.global()

lubyk = Autoload('lubyk')
-------------------------------- CONSTANTS
lubyk.service_type    = '_lubyk._tcp'
lubyk.info_url        = '/rk/info'
lubyk.add_service_url = '/rk/add_service'
lubyk.rem_service_url = '/rk/rem_service'

-------------------------------- lubyk.core
require 'lubyk.core'

-------------------------------- pack/unpack (msgpack.vendor)
require 'msgpack.vendor'

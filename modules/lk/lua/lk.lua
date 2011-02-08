--[[------------------------------------------------------

  rk
  -----

  This file is loaded just after the load paths have been
  changed and loads Lubyk essentials.

--]]------------------------------------------------------
require 'Autoload'
lk = Autoload('lk')
-- autoload stuff in _G
Autoload.global()

lubyk = Autoload('lubyk')
-------------------------------- CONSTANTS
lubyk.service_type    = '_lubyk._tcp'
lubyk.info_url        = '/lk/info'
lubyk.add_service_url = '/lk/add_service'
lubyk.rem_service_url = '/lk/rem_service'

-------------------------------- lubyk.core
require 'lubyk.core'

-------------------------------- pack/unpack (msgpack.vendor)
require 'msgpack.vendor'

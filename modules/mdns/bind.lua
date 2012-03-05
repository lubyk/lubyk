--[[------------------------------------------------------

  mdns bindings generator
  -----------------------

  This uses the 'dub' tool and Doxygen to generate the
  bindings for mdns.

  Input:  headers in 'include/mdns'
  Output: cpp files in 'core'

--]]------------------------------------------------------
require 'lubyk'

local base = lk.directory(lk.file())

local ins = dub.Inspector {
  INPUT    = base .. '/include/mdns',
}

local binder = dub.LuaBinder()

binder:bind(ins, {
  output_directory = base .. '/core',
  -- Remove this part in included headers
  header_base = base .. '/include',
  -- Execute all lua_open in a single go
  -- with lua_openmdns (creates mdns_core.cpp).
  single_lib = 'mdns_core',
  lib_prefix = false,
  only = {
    'mdns::Context',
    'mdns::Registration',
    'mdns::Browser',
    'mdns::Service',
  },
})

local s = ins:find('mdns::Service')

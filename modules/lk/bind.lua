--[[------------------------------------------------------

  lk bindings generator
  ---------------------

  This uses the 'dub' tool and Doxygen to generate the
  bindings for lk.

  Input:  headers in 'include/lk'
  Output: cpp files in 'sub'

--]]------------------------------------------------------
require 'lubyk'

local base = lk.directory(lk.file())

local ins = dub.Inspector {
  INPUT    = base .. '/include/lk',
}

local binder = dub.LuaBinder()

binder:bind(ins, {
  output_directory = base .. '/core',
  -- Remove this part in included headers
  header_base = base .. '/include',
  -- Execute all lua_open in a single go
  -- with lua_openmdns (creates mdns_core.cpp).
  single_lib = 'lk_core',
  lib_prefix = false,
})

local s = ins:find('mdns::Service')


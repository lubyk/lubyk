--[[------------------------------------------------------

  mdns
  ----

  ZeroConf (Bonjour) multi cast DNS bindings.

--]]------------------------------------------------------
-- This is required by mdns.Context on Linux. Make sure
-- Lua setup is done before the first lk.SelectCallback is
-- created from C++ and pushed into Lua. This adds yet
-- another dylib coupling: can't we avoid this ?

-- require 'lk.SelectCallback'
require 'mdns.core'
local core = mdns
mdns = Autoload('mdns')
mdns.core = core

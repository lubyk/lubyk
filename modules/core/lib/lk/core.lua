lk = {}
require 'lk.Autoload'
local Autoload = lk.Autoload
lk = lk.Autoload('lk')
lk.Autoload = Autoload
-- autoload stuff in _G
lk.Autoload.global()

require 'lk.core_c'
local time_ref = lk.TimeRef()
function elapsed()
  return time_ref:elapsed()
end
now = elapsed

function lk.bitTest(flags, bit)
  -- flags = [ 0 1 1 0 1 ] = 13
  -- bit   = [ 0 0 1 0 0 ] = 4
  -- 2bit  = [ 0 1 0 0 0 ] = 8
  -- 13 % (2*bit) => remove all bits above bit.
  return flags % (2*bit) >= bit
end

local CALL_TO_NEW = {__call = function(lib, ...) return lib.new(...) end}
function class(class_name, tbl)
  local lib = tbl or {}
  lib.type = class_name
  lib.__index = lib

  local base, klass = string.match(class_name, '^(.+)%.(.+)$')
  klass = klass or class_name

  if base and _G[base] then
    _G[base][klass] = lib
  end

  return setmetatable(lib, CALL_TO_NEW)
end

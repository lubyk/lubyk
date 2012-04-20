--[[------------------------------------------------------

  lk.crypto
  ---------

  Easy to use functions for common cryptography operations
  (uses 'crypto' library).

--]]------------------------------------------------------

local lib = {type='lk.crypto'}
lk.crypto = lib

local digest = crypto.digest
local sha1 = digest.new('sha1')
function lib.sha1(str)
  sha1:reset()
  return sha1:final(str)
end

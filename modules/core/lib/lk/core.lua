require 'Autoload'
lk    = Autoload('lk')
-- autoload stuff in _G
Autoload.global()

require 'lk.core_c'
local time_ref = lk.TimeRef()
function elapsed()
  return time_ref:elapsed()
end
now = elapsed

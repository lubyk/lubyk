--[[------------------------------------------------------

  # Automatic script reloading (live coding)

  In this tutorial, we show how to use lens.FileWatch to live code a lua script.

  ## Download source

  [LiveCoding.lua](example/lens/LiveCoding.lua)

--]]------------------------------------------------------
-- doc:lit

-- # Preamble
--
-- Require lens library.
local lens = require 'lens'

-- Start scheduler and setup script reload hook with `lens.FileWatch`. Starting
-- the scheduler at the top of the script and using file reloading is a nice
-- trick that ensures all the code after `lens.run` is only executed within the
-- scope of the scheduler.
lens.run(function() lens.FileWatch() end)

-- This part of the script is executed on the initial `FileWatch` call. This
-- means we are actually within the scheduler loop and we can therefore create
-- threads and timers.

-- # Do something
--
-- Here we create a timer for demonstration purpose but you could as well create
-- a window, a socket or whatever you need to do.
--
-- If the timer has not been created yet, create it. The syntax `tim or
-- lens.Timer(0.5)` ensures we are not creating a new timer on every script
-- reload.
--
-- Also of importance, note that we are not creating a `local` because local
-- variables are not seen on script reload.
tim = tim or lens.Timer(0.5)

-- Again, `i` is a global because of script reloading visibility.
i = i or 0

-- This timeout code demonstrates that lens.Timer does not drift.
function tim:timeout()
  i = i + 1
  local now = lens.elapsed()
  base = base or now
  print('timeout', i, math.floor(((now - base) * 1000) + 0.5)/1000)
end

-- You can play with the code by uncommenting this line or changing the timer's
-- `timeout` function.
tim:setInterval(0.5)
if not tim:running() then
  tim:start()
end

--coroutine.yield('gui')
coroutine.yield('halt')

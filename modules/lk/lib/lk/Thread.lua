--[[------------------------------------------------------

  lk.Thread
  ---------

  Ease the creation of new coroutines. These are *NOT* OS
  threads. If we find the need to create new OS threads,
  we will use:

  https://github.com/Neopallium/lua-llthreads

  Usage would be:

  data = lk.OSThread [=[
    return computed_data
  ]=] ==> yields. When thread finishes, execution 
  continues from here.

--]]------------------------------------------------------
local lib   = {type = 'lk.Thread'}
lib.__index = lib
lk.Thread   = lib

local WeakValue = {__mode = 'v'}

setmetatable(lib, {
  __call = function(lib, func)
    local self = {
      co = coroutine.create(func),
      should_run = true,
    }
    self.wrap = {
      -- weak link to thread
      t  = setmetatable({t = self}, WeakValue),
    }
    setmetatable(self, lib)
    sched:scheduleAt(0, self.wrap)
    return self
  end
})

-- The calling threads joins with this thread.
function lib:join()
  if self.co then
    coroutine.yield('join', self)
  else
    -- ignore: dead thread
  end
end

function lib:shouldRun()
  return self.should_run
end

function lib:kill()
  self.should_run = false
  self.co = nil
end

function lib:quit()
  self.should_run = false
end

--- @internal. The scheduler asks this thread to resume other threads
-- on finalization.
function lib:addJoin(thread_wrap)
  if not self.joins then
    self.joins = setmetatable({}, WeakValue)
  end
  table.insert(self.joins, thread_wrap)
end
  
function lib:finalize(scheduler)
  if self.joins then
    for _, thread_wrap in ipairs(self.joins) do
      -- joined
      scheduler:scheduleAt(0, thread_wrap)
    end
  end
end

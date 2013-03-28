--[[------------------------------------------------------

  lk.Timer
  --------

  The Timer contains a callback to execute a function at
  regular intervals.

  TODO: Do not trigger on start (makes it behave more like
  mimas.Timer). This means we have to change 'setInterval'.

--]]------------------------------------------------------
local lib   = {}
lib.__index = lib
lk.Timer    = lib

setmetatable(lib, {
  __call = function(lib, interval, func)
    -- FIXME: remove warning
    if interval > 10 then
      -- We changed interval from milliseconds to seconds.
      print(string.format("Using interval of %f seconds.\n%s\n", interval, debug.traceback()))
    end
    local self = {
      interval = interval,
    }
    self.cb = function() self:run() end
    if func then
      self.tick = func
    end
    return setmetatable(self, lib)
  end
})

function lib:tick() end

function lib:start(trigger)
  if trigger == false then
    -- do not trigger on start
    self.thread = lk.Thread(function()
      sched:wait(self.interval)
      self:run()
    end)
  else
    -- trigger on start (default)
    self.thread = lk.Thread(self.cb)
  end
end

function lib:run()
  if self.interval > 0 then
    while self.thread do
      local interval = self:tick()
      if interval then
        self:setInterval(interval)
        break
      else
        sched:wait(self.interval)
      end
    end
  end
end  

function lib:setInterval(interval, start_if_stopped)
  -- FIXME: remove warning
  if interval > 10 then
    -- We changed interval from milliseconds to seconds.
    printf("Using interval of %f seconds.\n%s\n", interval, debug.traceback())
  end
  if self.thread then
    -- Running timer: remove from scheduler.
    self:stop()
    if interval == 0 then
      -- Stop.
      self.interval = interval
      return
    end
  else
    -- Not running.
    self.interval = interval
    if start_if_stopped then
      self:start()
    end
    return
  end
  -- new trigger is
  -- LAST + k * interval (until > now)
  local n = 0
  if self.thread then
    n = self.thread.wrap.at or 0
  end
  if self.interval > 0 then
    -- Was running, move back to reschedule.
    while n > sched.now do
      n = n - self.interval
    end
  else
    n = sched.now
  end
  self.interval = interval

  -- n = last trigger time
  -- |.....|.....|..||...|
  -- n              now  dt
  local dt = interval - ((sched.now - n) % interval)
  self.thread = lk.Thread(self.cb, sched.now + dt)
end

function lib:join()
  if self.thread then
    self.thread:join()
  end
end

function lib:stop()
  if self.thread then
    self.thread:kill()
    self.thread = nil
  end
end

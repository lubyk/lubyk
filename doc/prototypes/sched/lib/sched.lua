local lib = {}
lib.__index = lib
local private = {}

setmetatable(lib, {
  __call = function()
    local self = {
      -- Ordered linked list of events by event time.
      at_next    = nil,
      -- List of threads that need to have their filedescriptor added to the
      -- select.
      fd_list    = {},
      -- These are plain lua functions that will be called when
      -- quitting.
      finalizers = {},
    }
    return setmetatable(self, lib)
  end
})

sched = Scheduler()

function lib:thread(func)
  local thread = {
    co = coroutine.create(func),
  }

  private.scheduleAt(0, thread)
end

function lib:wait(delay)
  if delay == 0 then
    coroutine.yield(0)
  else
    coroutine.yield(self.logical_now + delay)
  end
end

function lib:run()
  local at_list, fd_list = self.at_list, self.fd_list
  while true do
    local now = worker:now()
    local thread = self.at_top
    local run_at = thread.at
    local timeout
    if run_at >= 0 and run_at < now then
      -- remove from at_list
      self.at_top = thread.at_next
      -- run immediately 0 = run on every loop
      private.runThread(self, thread)
    elseif run_at > 0 then
      -- timeout
      timeout = now - run_at
    else
      -- no timeout
    end
    if not private.select(self, timeout) then
      break
    end
  end
  private.finalize(self)
end

function lib:addToSelect(thread, type)
  private.removeFromList(self, thread)
  if thread.read_fd then
    self:fdSet(thread.read_fd)
  end
  if thread.write_fd then
    self:fdSet(thread.write_fd)
  end
  if thread.error_fd then
    self:fdSet(thread.error_fd)
  end
  table.insert(self.fd_list, thread)
end

function lib:removeFromSelect(thread, type)
  private.removeFromList(self, thread)
  if thread.read_fd then
    self:fdClear(thread.read_fd)
  end
  if thread.write_fd then
    self:fdClear(thread.write_fd)
  end
  if thread.error_fd then
    self:fdClear(thread.error_fd)
  end
end

---

function private.removeFromList(thread)
  for i, t in ipairs(self.fd_list) do
    if t == thread then
      table.remove(fd_list, i)
    end
  end
end

function private:runThread(thread, what)
  if thread.at > 0 then
    self.logical_now = thread.at
  else
    self.logical_now = worker:now()
  end                
  -- FIXME: pcall ?
  local at = thread.co:resume(what)
  -- a value of 0 == execute again as soon as possible
  if at then
    private.scheduleAt(self, at, thread)
  end
end

function private:scheduleAt(at, thread)
  thread.at = at
  -- sorted insert (next event first)
  local prev = self
  while true do
    local ne = prev.at_next
    if not ne then
      prev.at_next = thread
      thread.at_next = nil
      break
    elseif at < ne.at then
      prev.at_next   = thread
      thread.at_next = ne
      break
    else
      prev = ne
    end
  end
end

function private:select(timeout)
  local count = self:select(timeout)
  if count == -1 then
    -- error
    return false
  end
  for _, thread in ipairs(self.fd_list) do
    if self.read_fd  and self:isSet(thread.read_fd) then
      -- read
      private.runThread(self, thread, 'read')
      count = count - 1
      if count == 0 then
        break
      end
    end

    if self.write_fd  and self:isSet(thread.write_fd) then
      -- write
      private.runThread(self, thread, 'write')
      count = count - 1
      if count == 0 then
        break
      end
    end

    if self.error_fd  and self:isSet(thread.error_fd) then
      -- error
      private.runThread(self, thread, 'error')
      count = count - 1
      if count == 0 then
        break
      end
    end
  end
end

function private:finalize()
  for _, func in ipairs(self.finalizers) do
    -- if one fails, should not crash all finalizers...
    pcall(func)
  end
end


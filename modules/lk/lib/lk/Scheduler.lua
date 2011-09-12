--[[------------------------------------------------------

  lk.Scheduler
  ------------

  Main event loop management. When we use a graphical
  interface, the scheduler automatically passes the
  loop to the GUI.

--]]------------------------------------------------------
local lib     = {}
lib.__index   = lib
lk.Scheduler  = lib
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

function lib:thread(func)
  local thread = {
    co = coroutine.create(func),
  }

  private.scheduleAt(self, 0, thread)
end

function lib:wait(delay)
  if delay == 0 then
    coroutine.yield(0)
  else
    coroutine.yield(self.logical_now + delay)
  end
end

function lib:waitRead(fd)         
  coroutine.yield(fd, 'read')
end

function lib:run()
  while true do
    local now = worker:now()
    local now_list = {}
    local timeout
    local thread = self.at_next
    while thread and thread.at < now do
      -- remove from at_list
      self.at_next = thread.at_next
      -- collect
      table.insert(now_list, thread)
      thread = self.at_next
    end
    self.at_next = thread

    for i, thread in ipairs(now_list) do
      -- We run them all now so that we give time for 'select' in
      -- case they keep on adding new events in at_next list.
      private.runThread(self, thread)
    end

    if self.at_next then
      -- timeout
      timeout = self.at_next.at - now
    end
      
    if private.select(self, timeout) == false then
      break
    end
  end
  private.finalize(self)
end

--=============================================== PRIVATE

function private:runThread(thread)
  if thread.at and thread.at > 0 then
    self.logical_now = thread.at
  else
    self.logical_now = worker:now()
  end                
  -- FIXME: pcall ?
  thread.at = nil
  local ok, a, b = coroutine.resume(thread.co)
  if not ok then
    -- a = error
    print('ERROR', a)
  elseif b then
    thread.fd = a
    thread.op = b -- read/write/error
    -- add thread to select
    -- FIXME: merge fdSet + insert
    table.insert(self.fd_list, thread)
    -- worker:fdReadSet(thread.fd)
    worker.fdSet[b](worker, a)
  elseif coroutine.status(thread.co) == 'dead' then
    -- let it repose in peace
    thread.co = nil
  else
    -- a = at
    -- a value of 0 == execute again as soon as possible
    if a then
      private.scheduleAt(self, a, thread)
    end
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
  local fd_list = self.fd_list
  if not timeout and #fd_list == 0 then
    -- nothing to do anymore (no events, no fd)
    return false
  end
  local count = worker:select(timeout or -1)  -- -1 == no timeout
  if count == -1 then
    -- error
    return false
  end
  for i, thread in ipairs(fd_list) do
    local fd = thread.fd
    if worker:fdReadIsSet(fd) then
      -- read
      table.remove(fd_list, i)
      worker:fdReadClear(fd)
      private.runThread(self, thread)
      count = count - 1
      if count == 0 then
        break
      end
    elseif worker:fdWriteIsSet(fd) then
      -- write
      table.remove(fd_list, i)
      worker:fdWriteClear(fd)
      private.runThread(self, thread)
      count = count - 1
      if count == 0 then
        break
      end
    elseif worker:fdErrorIsSet(fd) then
      -- error
      table.remove(fd_list, i)
      worker:fdErrorClear(fd)
      private.runThread(self, thread)
      count = count - 1
      if count == 0 then
        break
      end
    else
      -- nothing for this thread, leave it running
    end
  end
end

function private:finalize()
  for _, func in ipairs(self.finalizers) do
    -- if one fails, should not crash all finalizers...
    pcall(func)
  end
end

--=============================================== Without mimas

--=============================================== With mimas
